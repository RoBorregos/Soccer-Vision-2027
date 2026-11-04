
#include "BNO085.h"
#include <Adafruit_BNO08x.h>
#include <math.h>

BNO085::BNO085(Adafruit_BNO08x &hardwareInstance, TwoWire &bus, uint8_t i2cAddr,
               int interruptPin)
    : bno(hardwareInstance), // We bind our wrapper directly to the injected
                             // static memory
      i2cBus(bus), i2cAddr(i2cAddr), interruptPin(interruptPin), yaw(0.0f) {}

bool BNO085::init() {
    if (!bno.begin_I2C(i2cAddress, &i2cBus, interruptPin)) {
        return false;
    }

    if (!bno.enableReport(SH2_ROTATION_VECTOR, 10000)) {
        return false;
    }

    return true;
}

float BNO085::getYaw() { return this->yaw; }

void BNO085::update() {
    if (!bno.getSensorEvent(&sensorValue)) {
        return;
    }

    if (sensorValue.sensorId != SH2_ROTATION_VECTOR) {
        return;
    }

    float i = sensorValue.un.rotationVector.i;       // q1
    float j = sensorValue.un.rotationVector.j;       // q2
    float k = sensorValue.un.rotationVector.k;       // q3
    float real = sensorValue.un.rotationVector.real; // q0

    float yawRadians =
        atan2(2.0f * (real * k + i * j), real * real + i * i - j * j - k * k);

    this->yaw = yawRadians * 180.0f / PI;
}