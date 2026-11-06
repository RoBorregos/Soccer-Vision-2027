/**
 * @file test_config.hpp
 *
 * @brief common configuration
 */

#pragma once

#include "motor/Motor.hpp"
#include <Arduino.h>

static Motor motors[]{{29, 28, 2, false},
                      {35, 36, 5, false},
                      {31, 30, 3, false},
                      {34, 33, 4, false}};

constexpr int PWM = 100;
constexpr uint32_t TIME_MS = 1000;
constexpr uint32_t DELAY_MS = 500;
constexpr int START = 10; // seconds