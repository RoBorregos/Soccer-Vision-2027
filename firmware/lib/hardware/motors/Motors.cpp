/**
 * @file Motors.cpp
 *
 * @brief Motors Configuration
 *
 * @date 2026-11-01
 */

#include "Motors.hpp"
#include <math.h>

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

/*
void Motors::move(float degree, float speed, float rotation) {
    float pwm[kNumMotors];
    float maxPWM = 0.0f;

    for (size_t i = 0; i < kNumMotors; i++) {
        float angle = (degree - motors_[i]->getWheelAngle() - kOffset) *
                      kDegreesToRadians;
        pwm[i] = speed * cosf(angle) + rotation;
        maxPWM = fmaxf(maxPWM, fabsf(pwm[i]));
    }
    float scale = (maxPWM > kMaxPWM) ? kMaxPWM / maxPWM : 1.0f;

    for (size_t i = 0; i < kNumMotors; i++) {
        motors_[i]->setSpeed((int)lroundf(pwm[i] * scale));
    }
}*/

void Motors::move(float degree, float speed, float rotation) {
    float pwm[kNumMotors];
    float maxPWM = 0.0f;

    float angle1 = (degree - motors_[0]->getWheelAngle() - kOffset) * kDegreesToRadians;
    float angle2 = (degree - motors_[1]->getWheelAngle() - kOffset) * kDegreesToRadians;
    float angle3 = (degree - motors_[2]->getWheelAngle() - kOffset) * kDegreesToRadians;
    float angle4 = (degree - motors_[3]->getWheelAngle() - kOffset) * kDegreesToRadians;

    pwm[0] = speed * 0.90f * cosf(angle1) + rotation;
    pwm[1] = speed * 1.0f * cosf(angle2) + rotation;
    pwm[2] = speed * 1.0f * cosf(angle3) + rotation;
    pwm[3] = speed * 1.0f * cosf(angle4) + rotation;

    float scale = (maxPWM > kMaxPWM) ? kMaxPWM / maxPWM : 1.0f;

    for (size_t i = 0; i < kNumMotors; i++) {
        motors_[i]->setSpeed((int)lroundf(pwm[i] * scale));
    }
}

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