#ifndef BNO085_H
#define BNO085_H
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BNO08x.h>

class BNO085 {
    private:
        Adafruit_BNO08x bno;
        sh2_SensorValue_t sensorValue;
   
    public:
        BNO085();
        bool init();
        bool getYaw(float &yaw);
};
#endif