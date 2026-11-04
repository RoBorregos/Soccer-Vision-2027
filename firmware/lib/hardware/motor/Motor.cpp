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

Motor::Motor(uint8_t pinIn1, uint8_t pinIn2, uint8_t pinPWM, bool inverted,
             uint8_t encA, uint8_t encB, float diameter, float countsPerRev)
    : pinIn1_(pinIn1), pinIn2_(pinIn2), pinPWM_(pinPWM), inverted_(inverted),
      diameter_(diameter), countsPerRev_(countsPerRev_), encoder_(encA, encB) {}

void Motor::init() {
    pinMode(pinIn1_, OUTPUT);
    pinMode(pinIn2_, OUTPUT);
    pinMode(pinPWM_, OUTPUT);

    coast();
    resetEncoder();
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

int32_t Motor::readCount() {
    int32_t counts = encoder_.read();
    return encInverted_ ? -counts : counts;
}

int Motor::getEncoderCount() { return (int)readCount(); }

void Motor::resetEncoder() {
    encoder_.write(0);
    lastCount_ = 0;
    lastUs_ = micros();
    rps_ = 0.0f;
}

double Motor::getRotations() {
    return (double)readCount() / (double)countsPerRev_;
}

float Motor::getPosition() { return (float)getRotations() * M_PI * diameter_; }

void Motor::update() {
    uint32_t now = micros();
    uint32_t dt = now - lastUs_;
    if (dt < kMinWindowsUs)
        return;

    int32_t counts = readCount();
    rps_ = ((float)(counts - lastCount_) / countsPerRev_) /
           ((float)dt * kMicrosecondsToSeconds);

    lastCount_ = counts;
    lastUs_ = now;
}

float Motor::getRPS() const { return rps_; }

float Motor::getRPM() const { return rps_ * 60.0f; }

float Motor::getSpeedMPS() const { return rps_ * M_PI * diameter_; }

void Motor::setCounts(float counts) {
    if (counts > 0.0f)
        countsPerRev_ = counts;
}

void Motor::setEncoderInverted(bool inverted) { encInverted_ = inverted; }

void Motor::setPWMFrequency(float hz) { analogWriteFrequency(pinPWM_, hz); }