/**
 * @file test_motors.cpp
 *
 * @brief motors test
 */

#include "motors/Motors.hpp"
#include <Arduino.h>

static Motor fL{29, 28, 2, true};
static Motor fR{35, 36, 5, true};
static Motor bL{31, 30, 3, false};
static Motor bR{34, 33, 4, false};

static Motor *wheels[] = {&fL, &fR, &bL, &bR};

static Motors motors(fL, fR, bL, bR);

constexpr float kSpeed = 80.0f;
constexpr float kRotation = 60.0f;
constexpr uint32_t kMoveMS = 1500;

static void hold(float degree, float speed, float rotation, uint32_t ms) {
    uint32_t start = millis();
    while (millis() - start < ms) {
        motors.move(degree, speed, rotation);
        delay(5);
    }
    motors.coast();
}

static void oneByOne() {
    for (size_t i = 0; i < 4; i++) {
        uint32_t start = millis();
        while (millis() - start < kMoveMS) {
            wheels[i]->move(100);
            delay(5);
        }
        wheels[i]->coast();
        delay(500);
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000) {
    }
    motors.init();
}

void loop() {
    // One By One
    // oneByOne();

    // Rotate CCW+
    // hold(0,0,kRotation,kMoveMS);

    // Foward
    // hold(0,kSpeed,0,kMoveMS);

    // Right
    // hold(90,kSpeed,0,kMoveMS);

    // Backwards
    // hold(180,kSpeed,0,kMoveMS);

    // Left
    // hold(270,kSpeed,0,kMoveMS);
}