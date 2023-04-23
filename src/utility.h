//
// Created by Aden Prior on 19/4/2023.
//

#ifndef ROADS_UTILITY_H
#define ROADS_UTILITY_H

#include "utility.h"
#include "raylib.h"
#include "raymath.h"
#include <array>
#include <random>

int getRandomInt(int minValue, int maxValue);

int getRandomInt(size_t maxValue);

double distance(const Vector2 &a, const Vector2 &b);

bool closeEnough(float width, const Vector2 &a, const Vector2 &b);

#endif //ROADS_UTILITY_H
