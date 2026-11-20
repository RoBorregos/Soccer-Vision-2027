#include "constants/motorConstants.hpp"
#include "motor/Motor.hpp"
#include <Arduino.h>

Motor m{motorConstants::ids::frontRightIn1, motorConstants::ids::frontRightIn2,
        motorConstants::ids::frontRightPwm, false,
        motorConstants::angles::frontRight};

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