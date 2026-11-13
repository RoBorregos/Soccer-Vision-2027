
#include "MPU6050.h"
#include <math.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

MPU6050::MPU6050(Adafruit_MPU6050 &hardwareInstance, TwoWire &bus, uint8_t i2cAddr)
    : mpu(hardwareInstance),
      i2cBus(bus),
      i2cAddr(i2cAddr),
      gyro_bias_z(0.0f),
      yaw(0.0f),
      previous_time(0),
      previous_gyro(0.0f),{}

bool MPU6050::init() {
    int samples = 500;

    if (!mpu.begin(i2cAddr, &i2cBus)) {
        return false;
    }

    mpu.setGyroRange(MPU6050_RANGE_250_DEG);

    Serial.println("Calibrating gyro... DO NOT MOVE");

    float sum = 0.0f;

    sensors_event_t gyro;

    // Calibrate gyro Z
    unsigned long last_sample = millis();
    int i = 0;

    while (i < samples) {
        if (millis() - last_sample >= 5) {
            mpu.getEvent(nullptr, &gyro, nullptr);

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

float MPU6050::getYaw() {
    return this->yaw;
}

void MPU6050::update() {
    
    unsigned long current_time = micros();

    double dt = (current_time - previous_time) / 1000000.0;

    if (dt <= 0 || dt > 0.1) {
        dt = 0;
    }

    sensors_event_t gyro;

    mpu.getEvent(nullptr, &gyro, nullptr);

    // Convert rad/s to deg/s
    float gyro_z = gyro.gyro.z * 180.0f / PI;

    // Remove gyro bias
    gyro_z -= gyro_bias_z;

    // Deadband
    if (fabs(gyro_z) < 0.05f) {
        gyro_z = 0.0f;
    }

    // Integrate angular velocity
    avg_gyro_z = (gyro_z + previous_gyro) / 2.0f;
    this->yaw += avg_gyro_z * dt;
    previous_gyro = gyro_z;

    // Normalize yaw between -180 and 180
    if (this->yaw > 180.0f) { this->yaw -= 360.0f;}
    if (this->yaw < -180.0f) { this->yaw += 360.0f;}
    previous_time = current_time;
}
