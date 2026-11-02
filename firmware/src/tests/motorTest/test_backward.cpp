/**
 * @file test_backward.cpp
 *
 * @brief test motor cw- and negative encoder signal
 */

#include "test_common.hpp"
#include <Arduino.h>

void setup() {
    initTest("BACKWARD TEST");
    runOneByOne(-PWM, TIME_MS);
    runSame(-PWM, TIME_MS);
}

void loop() {}
