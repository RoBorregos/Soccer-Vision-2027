/**
 * @file test_heading.cpp
 *
 * @brief Test heading
 */

#include "BNO085.h"
#include "Motors.hpp"
#include "PIDController.h"
#include <Adafruit_BNO08x.h>
#include <Arduino.h>
#include <Wire.h>

static Motor fL{29, 28, 2, false};
static Motor fR{35, 36, 5, true};
static Motor bL{31, 30, 3, true};
static Motor bR{34, 33, 4, false};
static Motors motors(fL, fR, bL, bR);

static Adafruit_BNO08x native_bno_nav;
static BNO085 gyro(native_bno_nav, Wire, 0x4A, -1);

constexpr float kYawInverted = 1.0f;
constexpr float kMinOutput = 0.0f;
constexpr float kMaxOutput = 120.0f;
constexpr float kDeadbandDeg = 0.3f;
constexpr float kDriveSpeed = 160.0f;
constexpr uint32_t kLoopUs = 10000;
constexpr uint32_t kStaleMs = 500; // IMU debug
constexpr uint32_t kPrintMS = 50;

static PIDController pid(1.5f, 0.3f, 0.1f);
static bool enabled = true;
static bool drive = true;
static uint32_t lastLoopUs = 0;
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

    lastYaw = kYawInverted * gyro.getYaw();
    lastYawChangeMs = millis();

    pid.setTarget(lastYaw);
    pid.setOutputLimit(kMaxOutput);
    pid.setDeadband(kDeadbandDeg);
    pid.setMinOutput(kMinOutput);

    lastLoopUs = micros();
}

void loop() {
    gyro.update();
    uint32_t now = micros();
    if (now - lastLoopUs < kLoopUs)
        return;
    float dt = (now - lastLoopUs) * 0.000001f;
    lastLoopUs = now;

    float yaw = kYawInverted * gyro.getYaw();
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
            float targetSpeed = drive ? kDriveSpeed : 0.0f;
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
