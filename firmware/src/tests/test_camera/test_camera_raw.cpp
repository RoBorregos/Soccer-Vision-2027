/**
 * @file test_camera_raw.cpp
 *
 * @brief Echoes every byte received from the OpenMV (Serial1) to the USB
 *        serial monitor. Use it to check the UART wiring before parsing.
 */

#include <Arduino.h>

static uint32_t bytesReceived = 0;
static uint32_t lastReportMS = 0;

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200);
    while (!Serial && millis() < 3000) {
    }
    Serial.println("test_camera_raw: waiting for OpenMV data on Serial1 (pin 0 = RX1)");
}

void loop() {
    while (Serial1.available()) {
        Serial.write(Serial1.read());
        bytesReceived++;
    }

    if (millis() - lastReportMS > 2000) {
        lastReportMS = millis();
        // Loopback check: with a jumper between pin 1 (TX1) and pin 0 (RX1)
        // this line should be echoed back to the monitor.
        Serial1.print("teensy-loopback-ping\n");
        if (bytesReceived == 0)
            Serial.println("[no bytes received from Serial1 yet]");
    }
}
