#pragma once
#include "IMUInterface.h"
#include <Adafruit_BNO08x.h>
#include <Arduino.h>
#include <Wire.h>

class BNO085 : public IMUInterface {
  private:
    Adafruit_BNO08x &bno; // Reference to the static hardware memory
    TwoWire &i2cBus;

    float yaw;
    uint8_t i2cAddr;
    int interruptPin;

  public:
    // Constructor accepts the hardware instance reference
    BNO085(Adafruit_BNO08x &hardwareInstance, TwoWire &bus, uint8_t i2cAddr,
           int interruptPin);
    ~BNO085() override = default;

    bool init() override;
    void update() override;
    float getYaw() const override;
};
