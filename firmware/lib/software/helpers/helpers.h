#pragma once

constexpr float wrap(float value, float min, float max) {
    const float range = max - min;
    if (range <= 0.0f)
        return min; // Safety guard against infinite loops

    while (value > max)
        value -= range;
    while (value < min)
        value += range;
    return value;
}
