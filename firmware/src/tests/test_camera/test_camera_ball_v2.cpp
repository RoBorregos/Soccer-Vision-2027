/**
 * @file test_camera_ball_v2.cpp
 *
 * @brief v2: the robot drives towards the ball and strikes it
 */

#include "Camera.hpp"
#include "Motors.hpp"
#include "PIDController.h"
#include "constants/motorConstants.hpp"
#include "constants/pidConstants.hpp"
#include "helpers.h"
#include <Arduino.h>
#include <math.h>

namespace mc = motorConstants;
namespace pidc = pidConstants::ballTracking;

static Motor fL{mc::ids::frontLeftIn1, mc::ids::frontLeftIn2,
                mc::ids::frontLeftPwm, mc::inversions::frontLeft,
                mc::angles::frontLeft};
static Motor fR{mc::ids::frontRightIn1, mc::ids::frontRightIn2,
                mc::ids::frontRightPwm, mc::inversions::frontRight,
                mc::angles::frontRight};
static Motor bL{mc::ids::backLeftIn1, mc::ids::backLeftIn2,
                mc::ids::backLeftPwm, mc::inversions::backLeft,
                mc::angles::backLeft};
static Motor bR{mc::ids::backRightIn1, mc::ids::backRightIn2,
                mc::ids::backRightPwm, mc::inversions::backRight,
                mc::angles::backRight};

static Motors motors(fL, fR, bL, bR);

static Camera camera(Serial1);

constexpr float kMinRot = 0.0f;
constexpr float kMaxRot = 120.0f;
constexpr float kDeadband = 8.0f;

constexpr float kAngleSign = -1.0f;
constexpr float kMaxSteerDeg = 60.0f;
constexpr float kMaxSpeed = 140.0f;
constexpr float kMinSpeed = 70.0f;
constexpr float kAccelPerSec = 400.0f;
constexpr float kMaxDriveAngle = 50.0f;

constexpr float kSlowDist = 120.0f;
constexpr float kStrikeDist = 45.0f;

constexpr float kStrikeAngle = 10.0f;
constexpr float kStrikeSpeed = 220.0f;
constexpr uint32_t kStrikeMS = 350;
constexpr uint32_t kCooldownMS = 1000;

constexpr uint32_t kLostMS = 250;
constexpr uint32_t kPrintMS = 200;

enum class State { kSearch, kApproach, kStrike, kCooldown };

static PIDController pid(pidc::kP, pidc::kI, pidc::kD);

static State state = State::kSearch;
static uint32_t stateStartMS = 0;

static uint32_t lastFrame = 0;
static uint32_t lastFrameMS = 0;
static uint32_t lastSeenMS = 0;
static uint32_t lastLoopMS = 0;
static float rot = 0.0f;
static float lastDt = 0.0f;
static float currentSpeed = 0.0f;
static float targetSpeed = 0.0f;
static float driveDeg = 0.0f;

static const char *stateName(State s) {
    switch (s) {
    case State::kSearch:
        return "SEARCH";
    case State::kApproach:
        return "APPROACH";
    case State::kStrike:
        return "STRIKE";
    case State::kCooldown:
        return "COOLDOWN";
    }
    return "?";
}

static void setState(State s) {
    if (s == state)
        return;
    state = s;
    stateStartMS = millis();
    Serial.printf("[state] -> %s\n", stateName(s));
}

static float clampf(float v, float lo, float hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

static float approachSpeed(float dist, float angle) {
    float absAngle = fabsf(angle);
    if (absAngle > kMaxDriveAngle)
        return 0.0f;

    float t = clampf((dist - kStrikeDist) / (kSlowDist - kStrikeDist), 0.0f,
                     1.0f);
    float speed = kMinSpeed + (kMaxSpeed - kMinSpeed) * t;

    float align = cosf(absAngle * (float)M_PI / 180.0f);
    return speed * align;
}

static void rampSpeed(float dt) {
    float step = kAccelPerSec * dt;
    if (currentSpeed < targetSpeed)
        currentSpeed = fminf(targetSpeed, currentSpeed + step);
    else
        currentSpeed = fmaxf(targetSpeed, currentSpeed - step);
}

void setup() {
    Serial.begin(115200);
    camera.init();
    motors.init();

    pid.setTarget(0.0f); // ball centered
    pid.setOutputLimit(kMaxRot);
    pid.setMinOutput(kMinRot);
    pid.setDeadband(kDeadband);

    delay(3000);
    lastLoopMS = millis();
    stateStartMS = millis();
}

void loop() {
    camera.read();

    uint32_t now = millis();
    float loopDt = (now - lastLoopMS) * 0.001f;
    lastLoopMS = now;

    bool newFrame = camera.frameCount() != lastFrame;
    if (newFrame) {
        lastFrame = camera.frameCount();
        uint32_t frameMS = camera.lastUpdateMS();
        lastDt = (frameMS - lastFrameMS) * 0.001f;
        lastFrameMS = frameMS;
        if (camera.ballSeen)
            lastSeenMS = now;
    }

    bool ballVisible = (now - lastSeenMS) <= kLostMS;

    switch (state) {
    case State::kSearch:
        if (ballVisible)
            setState(State::kApproach);
        break;

    case State::kApproach:
        if (!ballVisible) {
            setState(State::kSearch);
            break;
        }
        if (newFrame && camera.ballSeen) {
            rot = pid.update(camera.ballAngle, lastDt);
            driveDeg = clampf(kAngleSign * camera.ballAngle, -kMaxSteerDeg,
                              kMaxSteerDeg);
            targetSpeed =
                approachSpeed(camera.ballDistance, camera.ballAngle);

            if (camera.ballDistance < kStrikeDist &&
                fabsf(camera.ballAngle) < kStrikeAngle)
                setState(State::kStrike);
        }
        break;

    case State::kStrike:
        if (now - stateStartMS > kStrikeMS)
            setState(State::kCooldown);
        break;

    case State::kCooldown:
        if (now - stateStartMS > kCooldownMS)
            setState(State::kSearch);
        break;
    }

    switch (state) {
    case State::kSearch:
    case State::kCooldown:
        pid.reset();
        rot = 0.0f;
        targetSpeed = 0.0f;
        currentSpeed = 0.0f;
        motors.coast();
        break;

    case State::kApproach:
        rampSpeed(loopDt);
        if (currentSpeed == 0.0f && rot == 0.0f)
            motors.coast();
        else
            motors.move(driveDeg, currentSpeed, -rot);
        break;

    case State::kStrike:
        // ball may leave the frame
        currentSpeed = kStrikeSpeed;
        motors.move(0.0f, kStrikeSpeed, 0.0f);
        break;
    }

    static uint32_t lastPrintMS = 0;
    if (now - lastPrintMS > kPrintMS) {
        lastPrintMS = now;
        if (!camera.fresh())
            Serial.printf("[%s] camera no data\n", stateName(state));
        else if (!camera.ballSeen)
            Serial.printf("[%s] data OK, ball not seen\n", stateName(state));
        else
            Serial.printf("[%s] dist=%.1f ang=%.1f dir=%.1f spd=%.0f rot=%.1f "
                          "dt=%.0fms\n",
                          stateName(state), camera.ballDistance,
                          camera.ballAngle, driveDeg, currentSpeed, rot,
                          lastDt * 1000.0f);
    }
}
