//imu_interface.h declaration
#pragma once

class IMUInterface {
  public:
    virtual ~IMUInterface() = default;
    virtual bool init() = 0;
    virtual void update() = 0; 
    virtual float getYaw() const = 0;
    
};