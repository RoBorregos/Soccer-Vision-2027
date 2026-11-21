/**
 * @file test_camera_ball.cpp
 *
 * @brief the robots rotates to the ball
 */

#include "Camera.hpp"
#include "Motors.hpp"
#include "constants/motorConstants.hpp"
#include "helpers.h"
#include <Arduino.h>

namespace mc = motorConstants;

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

constexpr float kKp = 1.5f;
constexpr float kMinRot = 60.0f;
constexpr float kMaxRot = 100.0f;
constexpr float kDeadband = 5.0f;
constexpr uint32_t kLostMS = 250;

static uint32_t lastSeenMS = 0;
static float lastError = 0;

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200);
    camera.init();
    motors.init();
    delay(3000);
}

void loop() {
    camera.read();
    if (camera.fresh() && camera.ballSeen) {
        lastSeenMS = millis();
        lastError = camera.ballAngle;
        Serial.println(camera.ballAngle);
    }

    if (millis() - lastSeenMS > kLostMS || fabsf(lastError) < kDeadband) {
        motors.coast();
        return;
    }

    float rot = constrain(kKp * fabsf(lastError), kMinRot, kMaxRot);
    if (lastError < 0)
        rot = -rot;

    motors.move(0.0f, 0.0f, -1.0f * rot);
}