//
// Created by Aden Prior on 19/4/2023.
//

#ifndef ROADS_MODEL_H
#define ROADS_MODEL_H

#include "raylib.h"
#include <utility>
#include <vector>
#include <memory>
#include <mutex>

struct Point {
    Vector2 position{};
    std::vector<std::shared_ptr<Point>> connectedPoints;
    bool isEntrance = false;
    bool isExit = false;
    Color color{};
};

using PointPtr = std::shared_ptr<Point>;
using Points = std::vector<PointPtr>;

// protects the path by setting up mutex
class Path {
public:
    Path() = default;

    void pop() {
        if (!path.empty()) {
            path.erase(path.begin());
        }
    }

    [[nodiscard]] PointPtr front() {
        return path.front();
    }

    void set(Points newPath) {
        path = std::move(newPath);
    }

    [[nodiscard]] bool empty() const {
        return path.empty();
    }

private:
    Points path;
};

class Car {
public:
    PointPtr currentPoint;
    PointPtr targetPoint;
    PointPtr destination;
    Vector2 position{};
    Path path;
    bool regeneratingPath = false;

    Car() {
        currentPoint = nullptr;
        targetPoint = nullptr;
        destination = nullptr;
    }
};

using CarPtr = std::shared_ptr<Car>;
using Cars = std::vector<CarPtr>;

#endif //ROADS_MODEL_H
