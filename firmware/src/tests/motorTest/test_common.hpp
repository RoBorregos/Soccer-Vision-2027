/**
 * @file test_common.hpp
 *
 * @brief iterate over the motor array
 */

#pragma once

#include "test_config.hpp"
#include <Arduino.h>

constexpr size_t MOTORS = sizeof(motors) / sizeof(motors[0]);

inline void stop() {
    for (size_t i = 0; i < MOTORS; i++) {
        motors[i].coast();
    }
}

inline void initTest(const char *name) {
    Serial.begin(115200);

    for (size_t i = 0; i < MOTORS; i++) {
        motors[i].init();
    }

    Serial.println(name);

    for (int s = START; s > 0; s--) {
        delay(1000);
    }
}

inline void run(const int speeds[], uint32_t ms) {
    float rps[MOTORS];

    for (size_t i = 0; i < MOTORS; i++) {
        motors[i].move(speeds[i]);
    }

    uint32_t n = millis();
    while (millis() - n < ms) {
        for (size_t i = 0; i < MOTORS; i++) {
            motors[i].update();
        }
    }

    stop();

    for (size_t i = 0; i < MOTORS; i++) {
        if (speeds[i] == 0)
            continue;
        Serial.print(speeds[i]);
    }

    delay(DELAY_MS);
}

inline void runSame(int speed, uint32_t ms) {
    int speeds[MOTORS];
    for (size_t i = 0; i < MOTORS; i++) {
        speeds[i] = speed;
    }

    run(speeds, ms);
}

inline void runOneByOne(int speed, uint32_t ms) {
    int speeds[MOTORS];

    for (size_t m = 0; m < MOTORS; m++) {
        for (size_t i = 0; i < MOTORS; i++) {
            speeds[i] = (i == m) ? speed : 0;
        }

        Serial.print("Motor ");
        Serial.println((int)m);
        run(speeds, ms);
    }
}