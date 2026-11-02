/**
 * @file test_forward.cpp
 *
 * @brief test motor ccw+ and positive encoder signal
 */

#include "test_common.hpp"
#include <Arduino.h>

void setup() {
    initTest("FORWARD TEST");
    runOneByOne(PWM, TIME_MS);
    runSame(PWM, TIME_MS);
}

void loop() {}
