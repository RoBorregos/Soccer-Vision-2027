#pragma once

#include <stdint.h>

namespace pidConstants {
namespace heading {
constexpr float kP = 1.5f;
constexpr float kI = 0.3f;
constexpr float kD = 0.1;
} // namespace heading

namespace ballTracking {
constexpr float kP = 0.3f;
constexpr float kI = 0.0f;
constexpr float kD = 0.1f;
} // namespace ballTracking
} // namespace pidConstants