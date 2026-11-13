// harware.cpp
#include <Adafruit_BNO08x.h>
#include <Adafruit_MPU6050.h>

// Linker places these directly into static RAM (.bss).
// This fully supports N independent BNO instances if needed.
Adafruit_MPU6050 native_mpu_nav;
Adafruit_BNO08x native_bno_nav;

