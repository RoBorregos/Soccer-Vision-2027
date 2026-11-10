/**
 * @file MotorInterface.h
 *
 * @brief Hardware abstraction
 */

#pragma once

class MotorInterface {
  public:
    virtual ~MotorInterface() = default;

    virtual void init() = 0;

    virtual void move(int speed) = 0;

    virtual void coast() = 0;

    virtual void brake() = 0;
};