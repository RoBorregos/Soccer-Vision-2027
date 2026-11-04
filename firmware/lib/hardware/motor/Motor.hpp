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
    Motor(uint8_t pinIn1, uint8_t pinIn2, uint8_t pinPWM, bool inverted,
          uint8_t encA, uint8_t encB, float diameter = 0.0f,
          float countsPerRev);

    Motor(const Motor &) = delete;
    Motor &operator=(const Motor &) = delete;

    void init();

    void move(int speed);
    void coast();
    void brake();

    int getEncoderCount();
    void resetEncoder();
    double getRotations();
    float getPosition(); // meters

    void update();
    float getRPS() const;
    float getRPM() const;
    float getSpeedMPS() const;

    void setCounts(float counts);
    void setEncoderInverted(bool inverted);
    void setPWMFrequency(float hz); // TODO: test on teensy

  private:
    const uint8_t pinIn1_, pinIn2_, pinPWM_;
    bool inverted_;
    float diameter_;

    float countsPerRev_ = 0.0f;
    bool encInverted_ = false;

    Encoder encoder_;

    int32_t lastCount_ = 0;
    uint32_t lastUs_ = 0;
    float rps_ = 0.0f;

    int32_t readCount();
};