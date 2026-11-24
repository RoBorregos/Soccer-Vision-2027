/**
 * @file test_camera_ball_v1.cpp
 *
 * @brief v1: the robot rotates in place to keep the ball centered (heading only)
 */

#include "Camera.hpp"
#include "Motors.hpp"
#include "PIDController.h"
#include "constants/motorConstants.hpp"
#include "constants/pidConstants.hpp"
#include "helpers.h"
#include <Arduino.h>

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

constexpr float kMinRot = 45.0f;
constexpr float kMaxRot = 120.0f;
constexpr float kDeadband = 8.0f;
constexpr uint32_t kLostMS = 250;
constexpr uint32_t kPrintMS = 200;

static PIDController pid(pidc::kP, pidc::kI, pidc::kD);

static uint32_t lastFrame = 0;
static uint32_t lastFrameMS = 0;
static uint32_t lastSeenMS = 0;
static float rot = 0.0f;
static float lastDt = 0.0f;

void setup() {
    Serial.begin(115200);
    camera.init();
    motors.init();

    pid.setTarget(0.0f); // ball centered
    pid.setOutputLimit(kMaxRot);
    pid.setMinOutput(kMinRot);
    pid.setDeadband(kDeadband);

    delay(3000);
}

void loop() {
    camera.read();
    
    if (camera.frameCount() != lastFrame) {
        lastFrame = camera.frameCount();
        uint32_t frameMS = camera.lastUpdateMS();
        lastDt = (frameMS - lastFrameMS) * 0.001f;
        lastFrameMS = frameMS;

        if (camera.ballSeen) {
            lastSeenMS = millis();
            rot = pid.update(camera.ballAngle, lastDt);
        }
    }

    static uint32_t lastPrintMS = 0;
    if (millis() - lastPrintMS > kPrintMS) {
        lastPrintMS = millis();
        if (!camera.fresh())
            Serial.println("[camera] no data");
        else if (!camera.ballSeen)
            Serial.println("[camera] data OK, ball not seen");
        else
            Serial.printf("[camera] ball dist=%.1f ang=%.1f rot=%.1f dt=%.0fms\n",
                          camera.ballDistance, camera.ballAngle, rot,
                          lastDt * 1000.0f);
    }

    if (millis() - lastSeenMS > kLostMS) {
        pid.reset();
        rot = 0.0f;
        motors.coast();
        return;
    }

    if (rot == 0.0f) { // inside deadband
        motors.coast();
        return;
    }

    motors.move(0.0f, 0.0f, -rot);
}
