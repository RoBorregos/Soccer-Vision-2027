/**
 * @file test_serial_scan.cpp
 *
 * @brief Listens on every Teensy 4.1 hardware serial port (Serial1..Serial8)
 *        and reports which one receives data. Use it to find the port the
 *        PCB routes the OpenMV to, without touching the wiring.
 */

#include <Arduino.h>

struct Port {
    HardwareSerial *serial;
    const char *name;
    uint8_t rxPin;
    uint8_t txPin;
    uint32_t bytes;
    char sample[40];
    size_t sampleLen;
};

static Port ports[] = {
    {&Serial1, "Serial1", 0, 1, 0, {0}, 0},
    {&Serial2, "Serial2", 7, 8, 0, {0}, 0},
    {&Serial3, "Serial3", 15, 14, 0, {0}, 0},
    {&Serial4, "Serial4", 16, 17, 0, {0}, 0},
    {&Serial5, "Serial5", 21, 20, 0, {0}, 0},
    {&Serial6, "Serial6", 25, 24, 0, {0}, 0},
    {&Serial7, "Serial7", 28, 29, 0, {0}, 0},
    {&Serial8, "Serial8", 34, 35, 0, {0}, 0},
};

static uint32_t lastReportMS = 0;

void setup() {
    Serial.begin(115200);
    for (auto &p : ports)
        p.serial->begin(115200);
    while (!Serial && millis() < 3000) {
    }
    Serial.println("test_serial_scan: listening on Serial1..Serial8 @115200");
}

void loop() {
    for (auto &p : ports) {
        while (p.serial->available()) {
            char c = (char)p.serial->read();
            p.bytes++;
            if (c == '\n' || c == '\r') {
                p.sampleLen = 0; // keep only the latest line
                continue;
            }
            if (p.sampleLen < sizeof(p.sample) - 1) {
                p.sample[p.sampleLen++] = (c >= 32 && c < 127) ? c : '?';
                p.sample[p.sampleLen] = '\0';
            }
        }
    }

    if (millis() - lastReportMS < 2000)
        return;
    lastReportMS = millis();

    Serial.println("---- serial scan ----");
    bool any = false;
    for (auto &p : ports) {
        if (p.bytes == 0)
            continue;
        any = true;
        Serial.printf("%s (RX pin %u, TX pin %u): %lu bytes | last: \"%s\"\n", p.name,
                      p.rxPin, p.txPin, (unsigned long)p.bytes, p.sample);
    }
    if (!any)
        Serial.println("no data on any port");
}
