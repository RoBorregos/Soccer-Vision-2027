
#include "MPU6050.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <helpers.h>
#include <math.h>

MPU6050::MPU6050(Adafruit_MPU6050 &hardwareInstance, TwoWire &bus,
                 uint8_t i2cAddr)
    : mpu(hardwareInstance), i2cBus(bus), i2cAddr(i2cAddr), gyro_bias_z(0.0f),
      yaw(0.0f), previous_time(0), previous_gyro(0.0f), change(0.0f),
      previous_change(0.0f) {}

bool MPU6050::init() {
    constexpr int samples = 500;

    if (!mpu.begin(i2cAddr, &i2cBus)) {
        return false;
    }

    mpu.setGyroRange(MPU6050_RANGE_250_DEG);

    Serial.println("Calibrating gyro... DO NOT MOVE");

    float sum = 0.0f;

    sensors_event_t gyro, accel, temp;

    // Calibrate gyro Z
    unsigned long last_sample = millis();
    int i = 0;

    while (i < samples) {
        unsigned long current_time = millis();
        if (current_time - last_sample >= 5) {
            mpu.getEvent(&accel, &gyro, &temp);

            float gyro_z = gyro.gyro.z * 180.0f / PI;

            sum += gyro_z;

            last_sample = millis();
            i++;
        }
    }

    gyro_bias_z = sum / samples;

    Serial.print("Gyro bias Z: ");
    Serial.println(gyro_bias_z);

    previous_time = micros();

    return true;
}

float MPU6050::getYaw() const { return this->yaw; }

void MPU6050::update() {

    unsigned long current_time = micros();
    constexpr float micro_to_second = 1000000.0f;

    double dt = (current_time - previous_time) / micro_to_second;

    if (dt <= 0 || dt > 0.1) { // If the time difference is too large or
                               // negative, reset dt to 0 as a safety measure
        dt = 0;
    }

    sensors_event_t gyro, accel, temp;

    mpu.getEvent(&accel, &gyro, &temp);

    // Convert rad/s to deg/s
    float gyro_z = gyro.gyro.z * 180.0f / PI;

    // Remove gyro bias
    gyro_z -= gyro_bias_z;
    change = gyro_z - previous_gyro;

    if (fabs(change - previous_change) > 0.5f) {
        // Deadband
        if (fabs(gyro_z) < 0.05f) {
            gyro_z = 0.0f;
        }

        // Integrate angular velocity
        float avg_gyro_z = (gyro_z + previous_gyro) / 2.0f;
        this->yaw += avg_gyro_z * dt;
        previous_gyro = gyro_z;

        // Normalize yaw between -180 and 180
        yaw = wrap(yaw, -180.0f, 180.0f);
    }

    previous_change = change;
    previous_time = current_time;
}
