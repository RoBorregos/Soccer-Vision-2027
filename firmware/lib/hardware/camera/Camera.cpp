/**
 * @file Camera.cpp
 *
 * @brief OpenMV frontal camera reader (UART)
 */

#include "Camera.hpp"
#include <math.h>
#include <stdio.h>

Camera::Camera(HardwareSerial &serial, bool isMirror, bool enemyYellow)
    : serial_(serial), isMirror_(isMirror), enemyYellow_(enemyYellow) {}

void Camera::init(uint32_t baud) { serial_.begin(baud); }

void Camera::read() {
    while (serial_.available()) {
        char c = (char)serial_.read();
        if (c == '\r')
            continue;
        if (c == '\n') {
            buffer_[len_] = '\0';
            len_ = 0;
            process(buffer_);
        } else if (len_ < kBufSize - 1) {
            buffer_[len_++] = c;
        } else {
            len_ = 0; // too long
        }
    }
}

void Camera::process(const char *line) {
    float dist, ang, area, yDist, yAng, bDist, bAng;
    if (sscanf(line, "%f %f %f %f %f %f %f", &dist, &ang, &area, &yDist, &yAng, &bDist,
               &bAng) != 7)
        return;

    ballDistance = dist;
    ballAngle = ang;

    if (enemyYellow_) {
        goalDistance = yDist;
        goalAngle = yAng;
        ownDistance = bDist;
        ownAngle = bAng;
    } else {
        goalDistance = bDist;
        goalAngle = bAng;
        ownDistance = yDist;
        ownDistance = yAng;
    }

    if (isMirror_)
        goalAngle += 25.0f;

    ballSeen = ballDistance > 1e-3f;
    goalSeen = goalDistance > 1e-3f;
    ownSeen = ownDistance > 1e-3f;

    if (goalSeen)
        lastValidBallAngle = goalAngle;

    lastUpdate_ = millis();
}

bool Camera::fresh(uint32_t timeoutMS) const {
    return (millis() - lastUpdate_) < timeoutMS;
}