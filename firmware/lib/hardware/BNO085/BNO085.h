#pragma once

#include <Adafruit_BNO08x.h>
#include <Arduino.h>
#include <Wire.h>

class BNO085 {
  private:
    Adafruit_BNO08x bno;
    sh2_SensorValue_t sensorValue; // can store different sensor values, but we
                                   // will use it for rotation vector
    float yaw;
    uint8_t i2cAddr;

  public:
    BNO085(uint8_t i2cAddr, int resetPin);
    bool init();
    float getYaw();
};
