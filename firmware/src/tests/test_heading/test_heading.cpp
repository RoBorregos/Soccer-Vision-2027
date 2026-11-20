/**
 * @file test_heading.cpp
 *
 * @brief Test heading
 */

#include "BNO085.h"
#include "Motors.hpp"
#include "PIDController.h"
#include "constants/motorConstants.hpp"
#include "constants/pidConstants.hpp"
#include <Adafruit_BNO08x.h>
#include <Arduino.h>
#include <Wire.h>

namespace mc = motorConstants;
namespace pidc = pidConstants::heading;

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

static Adafruit_BNO08x native_bno_nav;
static BNO085 gyro(native_bno_nav, Wire, 0x4A, -1);

constexpr float kMinOutput = 0.0f;
constexpr float kMaxOutput = 120.0f;
constexpr float kMinSteerOutput = 0.0f;
constexpr float kDeadbandDeg = 1.0f;
constexpr float kDriveSpeed = 160.0f;
constexpr uint32_t kLoopUs = 10000;
constexpr uint32_t kStaleMs = 500; // IMU debug
constexpr uint32_t kPrintMS = 50;
constexpr bool kUseMovementTimer = true;
constexpr uint32_t kMovementDurationMs = 1000;

static PIDController pid(pidc::kP, pidc::kI, pidc::kD);
static bool enabled = true;
static bool drive = true;
static uint32_t lastLoopUs = 0;
static uint32_t movementStartMs = 0;
static float lastYaw = 0.0f;
static uint32_t lastYawChangeMs = 0;
static uint32_t lastPrintMs = 0;

static float currentSpeed = 0.0f;
constexpr float kAccelPerSec = 250.0f;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    motors.init();

    fL.setPWMFrequency(20000);
    fR.setPWMFrequency(20000);
    bL.setPWMFrequency(20000);
    bR.setPWMFrequency(20000);

    if (!gyro.init()) {
        while (true) {
            motors.coast();
            delay(100);
        }
    }

    uint32_t start = millis();
    while (millis() - start < 1000) {
        gyro.update();
    }

    lastYaw = gyro.getYaw();
    lastYawChangeMs = millis();

    movementStartMs = millis();
    pid.setTarget(lastYaw);
    pid.setOutputLimit(kMaxOutput);
    pid.setDeadband(kDeadbandDeg);
    pid.setMinOutput(drive ? kMinOutput : kMinSteerOutput);

    lastLoopUs = micros();
}

void loop() {
    gyro.update();
    uint32_t now = micros();
    if (now - lastLoopUs < kLoopUs)
        return;
    float dt = (now - lastLoopUs) * 0.000001f;
    lastLoopUs = now;

    float yaw = gyro.getYaw();
    if (yaw != lastYaw) {
        lastYaw = yaw;
        lastYawChangeMs = millis();
    }

    float rot = 0.0f;
    if (enabled) {
        rot = pid.update(yaw, dt);

        if (rot != 0.0f && millis() - lastYawChangeMs > kStaleMs) {
            enabled = false;
            motors.coast();
            rot = 0.0f;
        } else {
            bool movementActive =
                drive && (!kUseMovementTimer ||
                          millis() - movementStartMs < kMovementDurationMs);
            float targetSpeed = movementActive ? kDriveSpeed : 0.0f;
            if (currentSpeed < targetSpeed) {
                currentSpeed =
                    min(targetSpeed, currentSpeed + kAccelPerSec * dt);
            } else {
                currentSpeed =
                    max(targetSpeed, currentSpeed - kAccelPerSec * dt);
            }
            motors.move(0.0f, currentSpeed, rot);
        }
    }

    while (millis() - lastPrintMs >= kPrintMS) {
        lastPrintMs = millis();
        Serial.println(yaw);
    }
}
