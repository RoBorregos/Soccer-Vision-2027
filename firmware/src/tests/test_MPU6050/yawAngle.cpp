#include "BNO085.h"
#include "MPU6050.h"
#include <Arduino.h>
#include <Wire.h>
#include <hardware.h>

extern class Adafruit_MPU6050 native_mpu;
MPU6050 gyro(native_mpu, Wire, 0x68);

void setup() {
    Wire.begin();
    Serial.begin(9600);
    if (gyro.init()) { // Initialize the MPU6050 sensor
        Serial.println("MPU6050 sensor initialized successfully.");
    } else {
        Serial.println("Failed to initialize MPU6050 sensor!");
    }
}

void loop() {
    Serial.print("Yaw: ");
    Serial.println(String(gyro.getYaw()));
    gyro.update(); // Update the sensor readings
    delay(50);
}
