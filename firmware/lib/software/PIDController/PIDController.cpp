/**
 * @file PIDController.cpp
 *
 * @brief PIDController Class
 */

#include "PIDController.h"
#include "../../../include/constants/pidConstants.hpp"
#include "../helpers/helpers.h"
#include <math.h>

namespace pidc = pidConstants::heading;

PIDController::PIDController(float kp, float ki, float kd)
    : kp_(kp), ki_(ki), kd_(kd) {}

void PIDController::setGains(float kp, float ki, float kd) {
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;
}

void PIDController::reset() {
    integral_ = 0.0f;
    rate_ = 0.0f;
    error_ = 0.0f;
    lastOutput_ = 0.0f;
    hasPrev_ = false;
}

void PIDController::setTarget(float degrees) { target_ = degrees; }
void PIDController::setOutputLimit(float maxOutput) { maxOutput_ = maxOutput; }
void PIDController::setDeadband(float degrees) { deadband_ = degrees; }
void PIDController::setMinOutput(float minOutput) { minOutput_ = minOutput; }

float PIDController::getTarget() const { return target_; }
float PIDController::getError() const { return error_; }
float PIDController::getKp() const { return kp_; }
float PIDController::getKi() const { return ki_; }
float PIDController::getKd() const { return kd_; }

float PIDController::update(float yawDeg, float dt) {
    if (dt <= 0.0f || dt > pidc::kMaxDt) {
        rate_ = 0.0f;
        hasPrev_ = false;
        return lastOutput_;
    }

    error_ = wrap(yawDeg - target_, -180.0f, 180.0f);

    if (hasPrev_) {
        float rate = wrap(yawDeg - lastYaw_, -180.0f, 180.0f) / dt;
        rate_ += pidc::kRateFilter * (rate - rate_);
    }
    lastYaw_ = yawDeg;
    hasPrev_ = true;

    if (fabsf(error_) < deadband_) {
        integral_ = 0.0f;
        lastOutput_ = 0.0f;
        return 0.0f;
    }

    if (ki_ > 0 && fabsf(error_) < pidc::kIntegralZoneDeg) {
        float maxIntegral = pidc::kMaxIntegralOutput / ki_;
        integral_ += error_ * dt;
        if (integral_ > maxIntegral) {
            integral_ = maxIntegral;
        }
        if (integral_ < -maxIntegral) {
            integral_ = -maxIntegral;
        }
    }

    float out = kp_ * error_ + ki_ * integral_ + kd_ * rate_;

    if (fabsf(out) < minOutput_)
        out = (out < 0.0f || (out == 0.0f && error_ < 0.0f)) ? -minOutput_
                                                             : minOutput_;

    if (out > maxOutput_)
        out = maxOutput_;
    if (out < -maxOutput_)
        out = -maxOutput_;

    lastOutput_ = out;
    return out;
}