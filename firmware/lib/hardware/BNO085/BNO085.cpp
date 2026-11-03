
#include "BNO085.h"
#include <math.h>

BNO085::BNO085(uint8_t i2cAddr = 0x4A, int resetPin = -1)
    : bno(resetPin),   // For reset pin, -1 means no reset pin, assuming I2C is
                       // used
      sensorValue{},   // Initialize the sensor value
      yaw(0.0f),       // Initialize yaw to 0
      i2cAddr(i2cAddr) // Store the I2C address
{}

bool BNO085::init() {
    if (!bno.begin_I2C(i2cAddr, &Wire)) {
        return false;
    }

    if (!bno.enableReport(SH2_ROTATION_VECTOR, 10000)) {
        return false;
    }

    return true;
}

float BNO085::getYaw() {

    if (!bno.getSensorEvent(&sensorValue)) { // get sensor event from BNO085,
                                             // should be SH2_ROTATION_VECTOR
        return this
            ->yaw; // return the last known yaw if no new data is available
    }

    if (sensorValue.sensorId !=
        SH2_ROTATION_VECTOR) { // checking for correct event type
        return this->yaw;
    }

    float i = sensorValue.un.rotationVector.i;       // q1
    float j = sensorValue.un.rotationVector.j;       // q2
    float k = sensorValue.un.rotationVector.k;       // q3
    float real = sensorValue.un.rotationVector.real; // q0

    float yawRadians =
        atan2(2.0f * (real * k + i * j), real * real + i * i - j * j - k * k);

    this->yaw = yawRadians * 180.0f / PI;
    return this->yaw;
}
