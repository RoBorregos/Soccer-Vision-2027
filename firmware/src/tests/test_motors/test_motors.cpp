/**
 * @file test_motors.cpp
 *
 * @brief motors test
 */

#include "constants/motorConstants.hpp"
#include "motors/Motors.hpp"
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

constexpr float kSpeed = 120.0f;
constexpr float kRotation = 40.0f;
constexpr uint32_t kMoveMS = 1500;

static void hold(float degree, float speed, float rotation, uint32_t ms) {
    motors.move(degree, speed, rotation);
    delay(ms);
    motors.coast();
}

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000) {
    }
    motors.init();

    fL.setPWMFrequency(20000);
    fR.setPWMFrequency(20000);
    bL.setPWMFrequency(20000);
    bR.setPWMFrequency(20000);
}

void loop() {
    hold(0, 0, kRotation, kMoveMS);

    // Foward
    // hold(0, kSpeed, 0, kMoveMS);

    // Right
    // hold(90,kSpeed,0,kMoveMS);

    // Backwards
    // hold(180,kSpeed,0,kMoveMS);

    // Left
    // hold(270,kSpeed,0,kMoveMS);
}