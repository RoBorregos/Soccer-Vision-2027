/**
 * @file Motor.hpp
 *
 * @brief Motor class
 *
 * @date 2026-11-01
 */

#pragma once

#include "../../software/MotorInterface/MotorInterface.h"
#include <Arduino.h>

class Motor : public MotorInterface {
  public:
    Motor(uint8_t pinIn1, uint8_t pinIn2, uint8_t pinPWM, bool inverted);

    Motor(const Motor &) = delete;
    Motor &operator=(const Motor &) = delete;

    void init() override;

    void move(int speed) override;
    void coast() override;
    void brake() override;

    void setPWMFrequency(float hz); // TODO: test on teensy
  private:
    const uint8_t pinIn1_, pinIn2_, pinPWM_;
    const bool inverted_;
};