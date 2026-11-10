/**
 * @file Motors.cpp
 *
 * @brief Motors Configuration
 *
 * @date 2026-11-01
 */

#include "Motors.hpp"
#include <math.h>

static const float kWheelDegrees[] = {322.5f, 37.5f, 217.5f, 142.5f};
static const float kOffset = 90.0f;
static const float kDegreesToRadians = M_PI / 180;
static const float kMaxPWM = 255.0f;

Motors::Motors(Motor &front_left, Motor &front_right, Motor &back_left,
               Motor &back_right)
    : motors_{&front_left, &front_right, &back_left, &back_right} {}

void Motors::init() {
    for (size_t i = 0; i < kNumMotors; i++) {
        motors_[i]->init();
    }
}

void Motors::move(float degree, float speed, float rotation) {
    float pwm[kNumMotors];
    float maxPWM = 0.0f;

    for (size_t i = 0; i < kNumMotors; i++) {
        float angle = (degree - kWheelDegrees[i] - kOffset) * kDegreesToRadians;
        pwm[i] = speed * cosf(angle) + rotation;
        maxPWM = fmaxf(maxPWM, fabsf(pwm[i]));
    }
    float scale = (maxPWM > kMaxPWM) ? kMaxPWM / maxPWM : 1.0f;

    for (size_t i = 0; i < kNumMotors; i++) {
        motors_[i]->move((int)lroundf(pwm[i] * scale));
    }
}

void Motors::rotate(float rotation) { move(0.0f, 0.0f, rotation); }

void Motors::moveForward(float speed) { move(0.0f, speed); }
void Motors::moveLeft(float speed) { move(270.0f, speed); }
void Motors::moveBackward(float speed) { move(180.0f, speed); }
void Motors::moveRight(float speed) { move(90.0f, speed); }

void Motors::coast() {
    for (size_t i = 0; i < kNumMotors; i++) {
        motors_[i]->coast();
    }
}

void Motors::brake() {
    for (size_t i = 0; i < kNumMotors; i++) {
        motors_[i]->brake();
    }
}