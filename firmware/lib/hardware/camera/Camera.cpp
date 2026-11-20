/**
 * @file Camera.cpp
 *
 * @brief OpenMV frontal camera reader (UART)
 */

#include "Camera.hpp"
#include <math.h>
#include <stdio.h>

Camera::Camera(HardwareSerial &serial, bool enemyYellow)
    : serial_(serial), enemyYellow_(enemyYellow) {}

void Camera::init(uint32_t baud) { serial_.begin(baud); }

void Camera::read() {
    while (serial_.available()) {
        char c = (char)serial_.read();
        if (c == '\r')
            continue;
        if (c == '\n') {
            buf_[len_] = '\0';
            len_ = 0;
            process(buf_);
        } else if (len_ < kBufSize - 1) {
            buf_[len_++] = c;
        } else {
            len_ = 0; // too long
        }
    }
}

void Camera::process(const char *line) {
    float bd, ba, yd, ya, blued, bluea;
    if(sscanf(line, "%f %f %f %f %f %f", &bd, &ba, &yd, &ya, &blued, &bluea) != 6) {
        return;
    }

    ballDistance = bd;
    ballAngle = ba;

    if(enemyYellow_) {
        goalDistance = yd;
        goalAngle = ya;
        ownDistance = blued;
        ownAngle = bluea;
    } else {
        goalDistance = blued;
        goalAngle = bluea;
        ownDistance = yd;
        ownDistance = ya;
    }

    ballSeen = ballDistance > 1e-3f;
    goalSeen = goalDistance > 1e-3f;
    lastUpdate_ = millis();
}

bool Camera::fresh(uint32_t timeoutMS) const {
    return(millis() - lastUpdate_) < timeoutMS;
}