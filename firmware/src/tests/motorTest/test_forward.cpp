/**
 * @file test_forward.cpp
 *
 * @brief test motor ccw+ and positive encoder signal
 */

#include <Arduino.h>
#include "test_common.hpp"

void setup()
{
    initTest("FORWARD TEST");
    runOneByOne(PWM, TIME_MS);
    runSame(PWM, TIME_MS);
}

void loop()
{
}
