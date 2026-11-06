/**
 * @file test_movement.cpp
 *
 * @brief test motor ccw+ and positive encoder signal
 */

#include "test_common.hpp"
#include <Arduino.h>

void setup() {
    initTest("TEST 1");
    runSame(PWM, TIME_MS);
}

void loop() {}
