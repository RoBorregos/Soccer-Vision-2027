#pragma once
#include "IMUInterface.h"
#include <Arduino.h>

class Adafruit_MPU6050;
class TwoWire;

class MPU6050 : public IMUInterface {
  private:
    Adafruit_MPU6050 &mpu; // Reference to the static hardware memory
    TwoWire &i2cBus;

    float yaw;
    uint8_t i2cAddr;
    float gyro_bias_z;
    unsigned long previous_time;
    float previous_gyro;

  public:
    // Constructor accepts the hardware instance reference
    MPU6050(Adafruit_MPU6050 &hardwareInstance, TwoWire &bus, uint8_t i2cAddr);
    ~MPU6050() override = default;

    bool init() override;
    void update() override;
    float getYaw() const override;
};
