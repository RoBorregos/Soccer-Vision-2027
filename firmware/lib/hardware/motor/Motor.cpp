/**
 * @file Motor.cpp
 *
 * @brief Motor Class
 *
 * @date 2026-11-01
 */

#include "Motor.hpp"

static const float kPI = 3.14159265f;
static const uint32_t kMinUS = 10000; // 10 ms

Motor::Motor(uint8_t pinIn1, uint8_t pinIn2, uint8_t pinPWM, bool inverted,
             uint8_t encA, uint8_t encB, float d)
    : pinIn1_(pinIn1), pinIn2_(pinIn2), pinPWM_(pinPWM), inverted_(inverted),
      diameter_(d), encoder_(encA, encB) {}

void Motor::init() {
    pinMode(pinIn1_, OUTPUT);
    pinMode(pinIn2_, OUTPUT);
    pinMode(pinPWM_, OUTPUT);

    coast();
    resetEncoder();
}

void Motor::move(int speed, Direction direction) {
    if (speed < 0)
        speed = 0;
    if (speed > 255)
        speed = 255;

    if (inverted_) {
        direction = (direction == Direction::FORWARD) ? Direction::BACKWARD
                                                      : Direction::FORWARD;
    }

    if (direction == Direction::FORWARD) {
        digitalWrite(pinIn1_, HIGH);
        digitalWrite(pinIn2_, LOW);
    } else {
        digitalWrite(pinIn1_, LOW);
        digitalWrite(pinIn2_, HIGH);
    }

    analogWrite(pinPWM_, speed);
}

void Motor::move(int speed) {
    if (speed > 0) {
        move(speed, Direction::FORWARD);
    } else if (speed < 0) {
        move(-speed, Direction::BACKWARD);
    } else {
        coast();
    }
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

int32_t Motor::readCount() {
    int32_t c = encoder_.read();
    return encInverted_ ? -c : c;
}

int Motor::getEncoderCount() { return (int)readCount(); }

void Motor::resetEncoder() {
    encoder_.write(0);
    lastCount_ = 0;
    lastUs_ = micros();
    rps_ = 0.0f;
}

double Motor::getRotations() { return (double)readCount() / (double)ppr_; }

float Motor::getPosition() { return (float)getRotations() * kPI * diameter_; }

void Motor::update() {
    uint32_t now = micros();
    uint32_t dt = now - lastUs_;
    if (dt < kMinUS)
        return;

    int32_t c = readCount();
    rps_ = ((float)(c - lastCount_) / ppr_) / ((float)dt * 0.000001);

    lastCount_ = c;
    lastUs_ = now;
}

float Motor::getRPS() const { return rps_; }

float Motor::getRPM() const { return rps_ * 60.0f; }

float Motor::getSpeedMPS() const { return rps_ * kPI * diameter_; }

void Motor::setCounts(float c) {
    if (c > 0.0f)
        ppr_ = c;
}

void Motor::setDiameter(float d) { diameter_ = d; }

void Motor::setEncoderInverted(bool i) { encInverted_ = i; }

void Motor::setPWMFrequency(float hz) { analogWriteFrequency(pinPWM_, hz); }