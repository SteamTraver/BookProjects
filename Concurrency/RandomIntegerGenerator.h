#pragma once
#include <random>

class RandomValueGenerator
{
public:
    explicit RandomValueGenerator(int min_, int max_)
        : min(min_), max(max_)
    {
    }

    RandomValueGenerator() = delete;
    RandomValueGenerator(const RandomValueGenerator&) = delete;
    RandomValueGenerator& operator=(const RandomValueGenerator&) = delete;

    int generate() const
    {
        std::random_device device;
        std::default_random_engine engine;
        std::uniform_int_distribution<int> distro(min, max);
        return distro(engine);
    }
private:
    int min;
    int max;
};