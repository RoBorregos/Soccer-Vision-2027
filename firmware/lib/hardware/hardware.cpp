// harware.cpp
#include <Adafruit_BNO08x.h>

// Linker places these directly into static RAM (.bss).
// This fully supports N independent BNO instances if needed.
Adafruit_BNO08x native_bno_nav;
