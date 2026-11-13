#include "Mock_IMU.h"

Mock_IMU::Mock_IMU() : yaw(0.0f) {}

bool Mock_IMU::init() { return true; }

void Mock_IMU::update() {}

float Mock_IMU::getYaw() const { return yaw; }

void Mock_IMU::setYaw(float value) { yaw = value; }