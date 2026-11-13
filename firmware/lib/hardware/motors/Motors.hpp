/**
 * @file Motors.hpp
 *
 * @brief Motors Class
 *
 * @date 2026-11-01
 */

#pragma once

#include "../motor/Motor.hpp"
#include <Arduino.h>

class Motors {
  public:
    Motors(Motor &front_left, Motor &front_right, Motor &back_left,
           Motor &back_right);

    void init();
    void move(float degree, float speed, float rotation = 0.0f);

    void coast();
    void brake();

  private:
    static constexpr size_t kNumMotors = 4;
    Motor *motors_[kNumMotors];

    static constexpr float kFrontLeftDeg = 322.5f;
    static constexpr float kFrontRightDeg = 37.5f;
    static constexpr float kBackLeftDeg = 217.5f;
    static constexpr float kBackRightDeg = 142.5f;

    static constexpr float kWheelDegrees[kNumMotors] = {
        kFrontLeftDeg, kFrontRightDeg, kBackLeftDeg, kBackRightDeg};
};