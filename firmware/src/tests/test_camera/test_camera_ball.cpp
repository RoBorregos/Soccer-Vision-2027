/**
 * @file test_camera_ball.cpp
 * 
 * @brief the robots rotates to the ball
 */

#include "Camera.hpp"
#include "helpers.h"
#include "constants/motorConstants.hpp"
#include "Motors.hpp"
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

static Camera camera(Serial2, true);

constexpr float kFrontAngle = 0.0f;
constexpr float kKp = 3.0f;
constexpr float kMinRot = 60.0f;
constexpr float kMaxRot = 120.0f;
constexpr float kDeadband = 6.0f;

void setup() {
    Serial.begin(115200);
    camera.init();
    motors.init();
    delay(3000);
}

void loop() {
    camera.read();
    if(!camera.fresh() || !camera.ballSeen) {
        motors.coast();
        return;
    }


    float error = wrap(camera.ballAngle - kFrontAngle, -180.0f, 180.0f);

    if(fabs(error) < kDeadband) {
        motors.coast();
        return;
    }

    float rot = constrain(kKp * fabs(error), kMinRot, kMaxRot);
    if(error < 0) rot = -rot;

    motors.move(0.0f,0.0f,rot);
}