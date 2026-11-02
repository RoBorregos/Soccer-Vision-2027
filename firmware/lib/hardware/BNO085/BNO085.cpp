
#include "BNO085.h"
#include <math.h>

BNO085::BNO085(string i2cPort, int interruptPin) {
     : bno(interruptPin) // For interrupt pin, -1 means no interrupt, assuming I2C is used
     if (init(i2cPort) == false) {
         Serial.println("Failed to initialize BNO085 sensor!");
     }
     else {
         Serial.println("BNO085 sensor initialized successfully.");
     }
     // Constructor
}

bool BNO085::init(string i2cPort) {
    if (!bno.begin_I2C(0x4A, &Wire)) {
        return false;
    }

    if (!bno.enableReport(SH2_ROTATION_VECTOR, 10000)) {
        return false;
    }

    return true;
}

bool BNO085::getYaw(float &yaw) {
    if (bno.wasReset()) {
        bno.enableReport(SH2_ROTATION_VECTOR, 10000);
    }

    if (!bno.getSensorEvent(&sensorValue)) {
        return false;
    }

    if (sensorValue.sensorId != SH2_ROTATION_VECTOR) {
        return false;
    }

    float i = sensorValue.un.rotationVector.i;       // q1
    float j = sensorValue.un.rotationVector.j;       // q2
    float k = sensorValue.un.rotationVector.k;       // q3
    float real = sensorValue.un.rotationVector.real; // q0

    float yawRadians =
        atan2(2.0f * (real * k - i * j), real * real + i * i - j * j - k * k);

    yaw = yawRadians * 180.0f / PI;

    return true;
}
