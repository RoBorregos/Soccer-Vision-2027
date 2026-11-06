/**
 * @file test_motors.cpp
 *
 * @brief test motors w/ motors class
 */

#include "../lib/hardware/motors/Motors.hpp"
#include "test_common.hpp"
#include <Arduino.h>

static Motor fL{29, 28, 2, true};
static Motor fR{35, 36, 5, true};
static Motor bL{31, 30, 3, false};
static Motor bR{34, 33, 4, false};

static Motors motorss(fL, fR, bL, bR);

constexpr float kSpeed = 100.0f;
constexpr uint32_t kMoveMs = 1000;
constexpr int kStart = 5;

void setup() {
    Serial.begin(115200);
    motorss.init();

    Serial.print("TEST 2");
}

void loop() {
    motorss.moveForward(kSpeed);
    delay(kMoveMs);
    motorss.coast();

    delay(1500);
}
