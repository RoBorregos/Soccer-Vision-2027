/**
 * @file Motor.hpp
 *
 * @brief Motor class
 *
 * @date 2026-11-01
 */

#pragma once

#include <Arduino.h>
#include <Encoder.h>

class Motor {
  public:
    Motor(uint8_t pinIn1, uint8_t pinIn2, uint8_t pinPWM, bool inverted);

    Motor(const Motor &) = delete;
    Motor &operator=(const Motor &) = delete;

    void init();

    void move(int speed);
    void coast();
    void brake();

    void update();

    void setMinPWM(uint8_t minPWM);
    void setRampPerSecond(float speedPerSecond);
    void setTimeoutMS(uint32_t ms);

    void setPWMFrequency(float hz); // TODO: test on teensy

  private:
    void apply(int speed);
    void writeCoast();

    const uint8_t pinIn1_, pinIn2_, pinPWM_;
    bool inverted_;

    uint8_t minPWM_ = 0;
    float rampPerSecond_ = 0.0f;
    uint32_t timeoutMS_ = 0;

    int target_ = 0;
    float current_ = 0.0f;

    uint32_t lastCmdMS_ = 0;
    uint32_t lastUs_ = 0;
};