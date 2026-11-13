/**
 * @file PIDController.h
 *
 * @brief PID keeping the robot on a heading
 */

#pragma once

class PIDController {
  public:
    PIDController(float kp, float ki, float kd);

    void setGains(float kp, float ki, float kd);
    void setTarget(float degrees);
    void setOutputLimit(float maxOutput);
    void setDeadband(float degrees);
    void setMinOutput(float minOutput);

    void reset();

    float update(float yawDeg, float dt);

    float getTarget() const;
    float getError() const;
    float getKp() const;
    float getKi() const;
    float getKd() const;

    static float wrap180(float deg);

  private:
    float kp_, ki_, kd_;
    float target_ = 0.0f;
    float maxOutput_ = 120.0f;
    float deadband_ = 1.0f;
    float minOutput_ = 0.0f;

    float integral_ = 0.0f;
    float rate_ = 0.0f;
    float lastYaw_ = 0.0f;
    float error_ = 0.0f;
    bool hasPrev_ = false;
};