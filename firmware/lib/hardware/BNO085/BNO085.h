#ifndef BNO085_H
#define BNO085_H
#include <Adafruit_BNO08x.h>
#include <Arduino.h>
#include <Wire.h>

class BNO085 {
  private:
    Adafruit_BNO08x bno;
    sh2_SensorValue_t sensorValue;

  public:
    BNO085(string i2cPort, int interruptPin);
    bool init(string i2cPort);
    bool getYaw(float &yaw);
};

#endif