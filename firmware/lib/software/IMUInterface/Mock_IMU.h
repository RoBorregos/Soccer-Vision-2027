#pragma once
#include "IMUInterface.h"

class Mock_IMU : public IMUInterface {
  private:
    float yaw;

  public:
    Mock_IMU();
    ~Mock_IMU() override = default;
    virtual bool init() override;
    virtual void update() override;
    virtual float getYaw() const override;
    virtual void setYaw(float newYaw);
};