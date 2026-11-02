#include "BNO085.h"
#include <Arduino.h>
#include <unity.h> //to unit test as it has various functions to test the sensors

BNO085 gyro("0x4A", -1);

void test_yaw_initial_position() {
    float yaw = 0;
    gyro.getYaw(yaw);
    TEST_ASSERT_FLOAT_WITHIN(5.0, 0.0, yaw);
}

void setup() {
    Serial.begin(9600);

    UNITY_BEGIN();

    RUN_TEST(test_yaw_initial_position);

    UNITY_END();
}

void loop() {
    Serial.print("Yaw: ");
    Serial.println(yaw);
    getYaw(yaw);
    delay(1000);
}