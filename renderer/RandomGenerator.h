#pragma once
#include <random>

class RandomGenerator {
private:
    std::random_device rd;
    std::mt19937 gen;
public:
    RandomGenerator() : gen(rd()) {}
    
    int getInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(gen);
    }

    float getFloat(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }

    double getDouble(double min, double max) {
        std::uniform_real_distribution<double> dist(min, max);
        return dist(gen);
    }

};

