/**
 * @file test_config.hpp
 * 
 * @brief common configuration
 */

#pragma once

#include <Arduino.h>
#include "motor/Motor.hpp"

static Motor motors[] {
    {0,0,0,false,0,0.0f}
};

constexpr int PWM = 100;
constexpr uint32_t TIME_MS = 1000;
constexpr uint32_t DELAY_MS = 500;
constexpr int START = 3; // seconds