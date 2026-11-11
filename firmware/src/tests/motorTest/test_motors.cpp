/**
 * @file test_motors.cpp
 *
 * @brief motors test
 */

#include "motors/Motors.hpp"
#include <Arduino.h>

static Motor fL{29, 28, 2, false};
static Motor fR{35, 36, 5, true};
static Motor bL{31, 30, 3, true};
static Motor bR{34, 33, 4, false};

static Motor *wheels[] = {&fL, &fR, &bL, &bR};

static Motors motors(fL, fR, bL, bR);

constexpr float kSpeed = 120.0f;
constexpr float kRotation = 80.0f;
constexpr uint32_t kMoveMS = 1500;

static void hold(float degree, float speed, float rotation, uint32_t ms) {
    motors.move(degree, speed, rotation);
    delay(ms);
    motors.coast();
}

static void oneByOne() {
    for (size_t i = 0; i < 4; i++) {
        wheels[i]->move(80);
        delay(kMoveMS);

        wheels[i]->coast();
        delay(500);
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000) {
    }
    motors.init();

    fL.setPWMFrequency(20000);
    fR.setPWMFrequency(20000);
    bL.setPWMFrequency(20000);
    bR.setPWMFrequency(20000);
}

void loop() {
    delay(3000);
    // One By One
    // oneByOne();

    // Rotate
    // hold(0,0,kRotation,kMoveMS);

    // Foward
    hold(0, kSpeed, 0, kMoveMS);

    // Right
    // hold(90,kSpeed,0,kMoveMS);

    // Backwards
    // hold(180,kSpeed,0,kMoveMS);

    // Left
    // hold(270,kSpeed,0,kMoveMS);
}