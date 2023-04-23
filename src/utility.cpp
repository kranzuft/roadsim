#include "utility.h"
#include "raylib.h"
#include "raymath.h"
#include <array>
#include <random>

// Utility function to generate random integers within the specified range
int getRandomInt(int minValue, int maxValue) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(minValue, maxValue);
    return dist(gen);
}

// Utility function to generate random integers within the specified range
int getRandomInt(size_t maxValue) {
    return getRandomInt(0, static_cast<int>(maxValue));
}

// Utility function to calculate the distance between two points
double distance(const Vector2 &a, const Vector2 &b) {
    return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

// Utility function to check if two points are close enough to each other
bool closeEnough(float width, const Vector2 &a, const Vector2 &b) {
    return distance(a, b) <= width;
}

