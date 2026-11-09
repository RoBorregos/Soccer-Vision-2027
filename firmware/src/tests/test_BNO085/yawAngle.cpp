#include "BNO085.h"
#include <Adafruit_BNO08x.h>
#include <Arduino.h>
#include <Wire.h>

Adafruit_BNO08x
    native_bno_nav; // not optimal should be a reference from the hardware.cpp
                    // file, but for testing purposes, we can use this for now
BNO085 gyro(native_bno_nav, Wire, 0x4A, -1);

void setup() {
    Wire.begin();
    Serial.begin(9600);
    if (gyro.init()) { // Initialize the BNO085 sensor
        Serial.println("BNO085 sensor initialized successfully.");
    } else {
        Serial.println("Failed to initialize BNO085 sensor!");
    }
}

void loop() {
    Serial.print("Yaw: ");
    Serial.println(String(gyro.getYaw()));
    gyro.update(); // Update the sensor readings
    delay(50);
}
