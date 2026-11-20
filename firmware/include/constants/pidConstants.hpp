#pragma once

#include <stdint.h>

namespace pidConstants {
namespace heading {
constexpr float kP = 1.5f;
constexpr float kI = 0.3f;
constexpr float kD = 0.1;
constexpr float kIntegralZoneDeg = 20.0f;
constexpr float kMaxIntegralOutput = 40.0f;
constexpr float kRateFilter = 0.3f;
constexpr float kMaxDt = 0.1f;
} // namespace heading
} // namespace pidConstants