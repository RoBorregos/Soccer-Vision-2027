/**
 * @file Motor.cpp
 *
 * @brief Motor Class
 *
 * @date 2026-11-01
 */

#include "Motor.hpp"

static const int kMaxSpeed = 255;
static const float kMicrosecondsToSeconds = 0.000001;
static const float kMaxDtSeconds = 0.05f;

Motor::Motor(uint8_t pinIn1, uint8_t pinIn2, uint8_t pinPWM, bool inverted)
    : pinIn1_(pinIn1), pinIn2_(pinIn2), pinPWM_(pinPWM), inverted_(inverted) {}

void Motor::init() {
    pinMode(pinIn1_, OUTPUT);
    pinMode(pinIn2_, OUTPUT);
    pinMode(pinPWM_, OUTPUT);

    coast();
    lastUs_ = micros();
}

void Motor::apply(int speed) {
    if (speed == 0) {
        writeCoast();
        return;
    }

    bool isForward = (speed > 0) ^ inverted_;
    digitalWrite(pinIn1_, isForward ? HIGH : LOW);
    digitalWrite(pinIn2_, isForward ? LOW : HIGH);

    int magnitude = constrain(abs(speed), 1, 255);
    if (minPWM_ > 0)
        magnitude =
            minPWM_ + (magnitude - 1) * (kMaxSpeed - minPWM_) / (kMaxSpeed - 1);

    analogWrite(pinPWM_, magnitude);
}

void Motor::writeCoast() {
    analogWrite(pinPWM_, 0);
    digitalWrite(pinIn1_, LOW);
    digitalWrite(pinIn2_, LOW);
}

void Motor::move(int speed) {
    target_ = constrain(speed, -kMaxSpeed, kMaxSpeed);
    lastCmdMS_ = millis();

    if (rampPerSecond_ <= 0.0f) {
        current_ = target_;
        apply(target_);
    }
}

void Motor::coast() {
    target_ = 0;
    current_ = 0.0f;
    writeCoast();
}

void Motor::brake() {
    target_ = 0;
    current_ = 0.0f;
    digitalWrite(pinIn1_, HIGH);
    digitalWrite(pinIn2_, HIGH);
    analogWrite(pinPWM_, 255);
}

void Motor::update() {
    uint32_t now = micros();
    uint32_t dt = (now - lastUs_) * kMicrosecondsToSeconds;
    lastUs_ = now;
    if (dt > kMaxDtSeconds)
        dt = kMaxDtSeconds;

    if (timeoutMS_ > 0 && target_ != 0 && millis() - lastCmdMS_ > timeoutMS_) {
        coast();
        return;
    }

    if (rampPerSecond_ <= 0.0f)
        return;

    float step = rampPerSecond_ * dt;
    if (current_ < target_)
        current_ = min(current_ + step, (float)target_);
    else if (current_ > target_)
        current_ = max(current_ - step, (float)target_);

    apply((int)lroundf(current_));
}

void Motor::setMinPWM(uint8_t minPWM) { minPWM_ = minPWM; }
void Motor::setRampPerSecond(float speedPerSecond) {
    rampPerSecond_ = speedPerSecond;
}
void Motor::setTimeoutMS(uint32_t ms) { timeoutMS_ = ms; }
void Motor::setPWMFrequency(float hz) { analogWriteFrequency(pinPWM_, hz); }