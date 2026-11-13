#include "motor/Motor.hpp"
#include <Arduino.h>

Motor m{35, 36, 5, false};

void setup() {
    Serial.begin(115200);
    m.init();
}

void loop() {
    delay(3000);

    m.setSpeed(100);
    delay(1000);
    m.coast();
    delay(1000);
    m.setSpeed(-100);
    delay(1000);
    m.coast();
    delay(1000);
}