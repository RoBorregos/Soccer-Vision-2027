/**
 * @file test_common.hpp
 *
 * @brief iterate over the motor array
 */

#pragma once

#include "test_config.hpp"
#include <Arduino.h>

constexpr size_t MOTORS = sizeof(motors) / sizeof(motors[0]);
constexpr int32_t MIN_ENC = 10; // encoder debug

inline void stop() {
  for (size_t i = 0; i < MOTORS; i++) {
    motors[i].coast();
  }
}

inline const char *verdict(int speed, int32_t counts) {
  int32_t mag = (counts < 0) ? -counts : counts;

  if (mag < MIN_ENC)
    return "NONE";
  if ((counts > 0) != (speed > 0))
    return "INVERT";
  return "OK";
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
  int32_t counts[MOTORS];
  float rps[MOTORS];

  for (size_t i = 0; i < MOTORS; i++) {
    motors[i].resetEncoder();
    motors[i].move(speeds[i]);
  }

  uint32_t n = millis();
  while (millis() - n < ms) {
    for (size_t i = 0; i < MOTORS; i++) {
      motors[i].update();
    }
  }

  for (size_t i = 0; i < MOTORS; i++) {
    counts[i] = motors[i].getEncoderCount();
    rps[i] = motors[i].getRPS();
  }

  stop();
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