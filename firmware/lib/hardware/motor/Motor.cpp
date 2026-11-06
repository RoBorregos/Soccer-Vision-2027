/**
 * @file Motor.cpp
 *
 * @brief Motor Class
 *
 * @date 2026-11-01
 */

#include "Motor.hpp"

static const uint32_t kMinWindowsUs = 10000; // 10 ms
static const float kMicrosecondsToSeconds = 0.000001;

Motor::Motor(uint8_t pinIn1, uint8_t pinIn2, uint8_t pinPWM, bool inverted)
    : pinIn1_(pinIn1), pinIn2_(pinIn2), pinPWM_(pinPWM), inverted_(inverted) {}

void Motor::init() {
    pinMode(pinIn1_, OUTPUT);
    pinMode(pinIn2_, OUTPUT);
    pinMode(pinPWM_, OUTPUT);

    coast();
}

void Motor::move(int speed) {
    if (speed == 0) {
        coast();
        return;
    }

    bool isForward = (speed > 0) ^ inverted_;
    digitalWrite(pinIn1_, isForward ? HIGH : LOW);
    digitalWrite(pinIn2_, isForward ? LOW : HIGH);

    analogWrite(pinPWM_, constrain(abs(speed), 0, 255));
}

void Motor::coast() {
    analogWrite(pinPWM_, 0);
    digitalWrite(pinIn1_, LOW);
    digitalWrite(pinIn2_, LOW);
}

void Motor::brake() {
    digitalWrite(pinIn1_, HIGH);
    digitalWrite(pinIn2_, HIGH);
    analogWrite(pinPWM_, 255);
}

void Motor::update() {
    uint32_t now = micros();
    uint32_t dt = now - lastUs_;
    if (dt < kMinWindowsUs)
        return;

    lastUs_ = now;
}

void Motor::setPWMFrequency(float hz) { analogWriteFrequency(pinPWM_, hz); }