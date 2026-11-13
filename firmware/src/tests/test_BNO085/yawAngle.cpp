#include "BNO085.h"
#include <Arduino.h>
#include <Wire.h>
#include <hardware.h>

extern class Adafruit_BNO08x native_bno_nav;
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
