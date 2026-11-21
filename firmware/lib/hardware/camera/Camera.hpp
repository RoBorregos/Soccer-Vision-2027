/**
 * @file Camera.hpp
 *
 * @brief OpemMV frontal camera reader (UART)
 */

#pragma once

#include <Arduino.h>

class Camera {
  public:
    Camera(HardwareSerial &serial, bool isMirror = false,
           bool enemyYellow = true);

    void init(uint32_t baud = 115200);
    void read();

    float ballDistance = 0, ballAngle = 0;
    float goalDistance = 0, goalAngle = 0;
    float ownDistance = 0, ownAngle = 0;

    bool ballSeen = false, goalSeen = false, ownSeen = false;
    float lastValidBallAngle = 0;

    bool fresh(uint32_t timeoutMS = 300) const;

  private:
    static constexpr size_t kBufSize = 120;

    HardwareSerial &serial_;
    bool isMirror_;
    bool enemyYellow_;
    char buffer_[kBufSize];
    size_t len_ = 0;
    uint32_t lastUpdate_ = 0;

    void process(const char *line);
};