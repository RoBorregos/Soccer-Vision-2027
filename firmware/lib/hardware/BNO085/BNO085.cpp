
#include "BNO085.h"
#include <math.h>

BNO085::BNO085(string i2cPort, int interruptPin) {
     : bno(interruptPin) // For interrupt pin, -1 means no interrupt, assuming I2C is used
     sensorValue = {}; // Initialize the sensor value
     yaw = 0.0f; // Initialize yaw to 0
     this->i2cPort = i2cPort;
}

bool BNO085::init() {
    if (!bno.begin_I2C(i2cPort, &Wire)) {
        return false;
    }

    if (!bno.enableReport(SH2_ROTATION_VECTOR, 10000)) {
        return false;
    }

    return true;
}

; // Global variable to hold the yaw value

float BNO085::getYaw() {

    if (!bno.getSensorEvent(&sensorValue)) { // get sensor event from BNO085,
                                             // should be SH2_ROTATION_VECTOR
        this->yaw = 0;
        Serial.println("Failed to get sensor event from BNO085.");
        return 0;
    }

    if (sensorValue.sensorId !=
        SH2_ROTATION_VECTOR) { // checking for correct event type
        this->yaw = 0;
        Serial.println("Received sensor event is not a rotation vector.");
        return 0;
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
