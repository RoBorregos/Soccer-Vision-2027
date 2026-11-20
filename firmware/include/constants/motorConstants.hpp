#pragma once

#include <stdint.h>

namespace motorConstants {
namespace ids {
constexpr uint8_t frontLeftIn1 = 29;
constexpr uint8_t frontLeftIn2 = 28;
constexpr uint8_t frontLeftPwm = 2;
constexpr uint8_t frontRightIn1 = 35;
constexpr uint8_t frontRightIn2 = 36;
constexpr uint8_t frontRightPwm = 5;
constexpr uint8_t backLeftIn1 = 31;
constexpr uint8_t backLeftIn2 = 30;
constexpr uint8_t backLeftPwm = 3;
constexpr uint8_t backRightIn1 = 34;
constexpr uint8_t backRightIn2 = 33;
constexpr uint8_t backRightPwm = 4;
} // namespace ids

namespace angles {
constexpr float frontLeft = 322.5f;
constexpr float frontRight = 37.5f;
constexpr float backLeft = 217.5f;
constexpr float backRight = 142.5f;
} // namespace angles

namespace inversions {
constexpr bool frontLeft = false;
constexpr bool frontRight = true;
constexpr bool backLeft = true;
constexpr bool backRight = false;
} // namespace inversions
} // namespace motorConstants