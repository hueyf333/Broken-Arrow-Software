#pragma once
#include <random>
#include <cstdint>

namespace BrokenArrow {
namespace Math {

class Random {
public:
    Random(uint32_t seed = std::random_device{}()) : rng_(seed), dist_(0.0f, 1.0f) {}

    void SetSeed(uint32_t seed) {
        rng_.seed(seed);
    }

    float Range(float min, float max) {
        return min + (max - min) * dist_(rng_);
    }

    int RangeInt(int min, int max) {
        std::uniform_int_distribution<int> intDist(min, max);
        return intDist(rng_);
    }

    float Value() {
        return dist_(rng_);
    }

    bool Bool(float probability = 0.5f) {
        return Value() < probability;
    }

    static Random& Global() {
        static Random instance;
        return instance;
    }

private:
    std::mt19937 rng_;
    std::uniform_real_distribution<float> dist_;
};

} // namespace Math
} // namespace BrokenArrow
