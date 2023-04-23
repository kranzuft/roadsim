#include "raylib.h"
#include "raymath.h"
#include <memory>
#include <array>
#include <unordered_map>
#include <cfloat>
#include "model.h"
#include "AStar.h"
#include "utility.h"
#include "ui.h"

#define RAYGUI_IMPLEMENTATION

#include "raygui.h"

const std::array<Color, 6> exitColors = {
        {
                {0, 250, 0, 255},      // Green
                {0, 0, 250, 255},      // Blue
                {150, 0, 200, 255},    // Purple
                {255, 255, 0, 255},    // Yellow
                {70, 150, 230, 255} // Light blue
        }
};

PointPtr handleKeyS(Vector2 mousePosition) {
    auto entrance = std::make_shared<Point>();
    entrance->position = mousePosition;
    entrance->isEntrance = true;
    entrance->isExit = false;
    return entrance;
}

void handleKeyE(Points &points, Vector2 mousePosition, std::array<bool, exitColors.size()> &colorsUsed) {
    size_t availableColorIndex = -1;
    for (size_t i = 0; i < colorsUsed.size(); ++i) {
        if (!colorsUsed[i]) {
            availableColorIndex = i;
            break;
        }
    }

    if (availableColorIndex != -1) {
        auto exit = std::make_shared<Point>();
        exit->position = mousePosition;
        exit->isEntrance = false;
        exit->isExit = true;
        exit->color = exitColors[availableColorIndex];
        points.push_back(exit);
        colorsUsed[availableColorIndex] = true;
    }
}

std::optional<CarPtr> createCar(const PointPtr &entrance, const Points &exits, Points &points) {
    Points potentialExits;

    for (const auto &potentialExit: exits) {
        auto path = aStar(entrance, potentialExit, points);
        if (!path.empty()) {
            potentialExits.push_back(potentialExit);
        }
    }

    if (!potentialExits.empty()) {
        PointPtr exit = potentialExits[getRandomInt(potentialExits.size() - 1)];
        CarPtr newCar = std::make_shared<Car>();
        newCar->currentPoint = entrance;
        newCar->destination = exit;
        newCar->position = newCar->currentPoint->position;
        return {newCar};
    }

    return std::nullopt;
}

void updateCar(const CarPtr& car, Points &points, float deltaTime, float carSpeed) {
    if (!car->path.empty() && !car->regeneratingPath) {
        PointPtr nextPoint = car->path.front();
        if (distance(car->position, nextPoint->position) <= carSpeed * deltaTime) {
            car->position = nextPoint->position;
            car->path.pop();
            car->regeneratingPath = true;
        } else {
            Vector2 direction = Vector2Subtract(nextPoint->position, car->position);
            direction = Vector2Normalize(direction);
            direction = Vector2Scale(direction, carSpeed * deltaTime);
            car->position = Vector2Add(car->position, direction);
        }
    } else {
        // Create thread to update path

        if (car->regeneratingPath) {
            car->path.set(aStar(car->currentPoint, car->destination, points));
            car->regeneratingPath = false;
        } else if (car->path.empty()) {
            car->path.set(aStar(car->currentPoint, car->destination, points));
        }

        if (!car->path.empty()) {
            car->currentPoint = car->path.front();
            car->path.pop();
        }
    }
}

void updateCars(Cars &cars, Points &points, float deltaTime, const Settings &settings, float &spawnTimer) {
    spawnTimer += deltaTime;

    Points entrances;
    Points exits;

    if (spawnTimer >= settings.carSpawnInterval) {
        for (const auto &point: points) {
            if (point->isEntrance) {
                entrances.push_back(point);
            } else if (point->isExit) {
                exits.push_back(point);
            }
        }

        if (!entrances.empty() && !exits.empty()) {
            // loop through entrances:
            for (const auto &entrance: entrances) {
                if (getRandomInt(entrances.size() / 2 + 2) == 0 || cars.empty()) {
                    auto maybeCar = createCar(entrance, exits, points);
                    if (maybeCar.has_value()) {
                        cars.push_back(maybeCar.value());
                    }
                }
            }
            spawnTimer = 0;
        }
    }

    Cars carsToRemove;
    // Use an iterator to loop through the cars
    for (const auto& car : cars) {
        updateCar(car, points, deltaTime, settings.carSpeed);
    }
}

void drawCar(const CarPtr &car, float carLength) {
    DrawCircleV(car->position, carLength, car->destination->color);
}

void drawCars(const Cars &cars, float carLength) {
    for (const auto &car: cars) {
        drawCar(car, carLength);
    }
}

void handleLeftMousePressFirstPoint(Points &points, Vector2 mousePosition, PointPtr &currentPoint, float roadWidth) {
    bool existingPointFound = false;
    // look for point to connect to first
    for (const auto &point: points) {
        if (closeEnough(roadWidth, point->position, mousePosition)) {
            currentPoint = point;
            existingPointFound = true;
            break;
        }
    }

    // If no point found, create a new one
    if (!existingPointFound) {
        currentPoint = std::make_shared<Point>();
        currentPoint->position = mousePosition;
        currentPoint->isEntrance = false;
        currentPoint->isExit = false;
        points.push_back(currentPoint);
    }
}

bool lineIntersection(Vector2 p1, Vector2 p2, Vector2 q1, Vector2 q2, Vector2 &intersection) {
    Vector2 r = Vector2Subtract(p2, p1);
    Vector2 s = Vector2Subtract(q2, q1);

    float rxs = r.x * s.y - r.y * s.x;
    float qpxr = (q1.x - p1.x) * r.y - (q1.y - p1.y) * r.x;

    // If r x s = 0 and (q - p) x r = 0, then the two lines are collinear.
    if (fabs(rxs) < FLT_EPSILON && fabs(qpxr) < FLT_EPSILON) {
        return false;
    }

    // If r x s = 0 and (q - p) x r != 0, then the two lines are parallel and non-intersecting.
    if (fabs(rxs) < FLT_EPSILON && fabs(qpxr) >= FLT_EPSILON) {
        return false;
    }

    float t = ((q1.x - p1.x) * s.y - (q1.y - p1.y) * s.x) / rxs;
    float u = ((q1.x - p1.x) * r.y - (q1.y - p1.y) * r.x) / rxs;

    if ((FLT_EPSILON <= t && t <= 1.0f - FLT_EPSILON) && (FLT_EPSILON <= u && u <= 1.0f - FLT_EPSILON)) {
        intersection = Vector2Add(p1, Vector2Scale(r, t));
        return true;
    }

    return false;
}

void handleLeftMousePressNextPoint(Points &points, Vector2 mousePosition,
                                   PointPtr &currentPoint,
                                   float roadWidth) {
    bool merged = false;

    // Find a nearby point to connect to
    for (const auto &point : points) {
        if (closeEnough(roadWidth, point->position, mousePosition)) {
            currentPoint->connectedPoints.push_back(point);
            currentPoint = point;
            merged = true;
            break;
        }
    }

    Vector2 intersection;
    if (!merged) {
        // Iterate through all points and their connected points for intersections
        for (const auto &point : points) {
            for (auto it = point->connectedPoints.begin(); it != point->connectedPoints.end(); ++it) {
                if (lineIntersection(currentPoint->position, mousePosition, point->position, (*it)->position, intersection)) {
                    auto intersectPoint = std::make_shared<Point>();
                    intersectPoint->position = intersection;
                    intersectPoint->isEntrance = false;
                    intersectPoint->isExit = false;

                    currentPoint->connectedPoints.push_back(intersectPoint);
                    intersectPoint->connectedPoints.push_back(*it);

                    point->connectedPoints.erase(it);
                    point->connectedPoints.push_back(intersectPoint);

                    points.push_back(intersectPoint);
                    currentPoint = intersectPoint;
                }
            }
        }
    }

    // If no nearby point or intersection is found, create a new point
    if (!merged) {
        auto newPoint = std::make_shared<Point>();
        newPoint->position = mousePosition;
        newPoint->isEntrance = false;
        newPoint->isExit = false;

        currentPoint->connectedPoints.push_back(newPoint);

        points.push_back(newPoint);
        currentPoint = newPoint;
    }
}

// Handle left mouse button press events to draw roads and connect points.
void handleLeftMousePress(Points &points, Vector2 mousePosition, PointPtr &currentPoint, float roadWidth) {
    if (mousePosition.x < 0 || mousePosition.x > static_cast<float>(GetScreenWidth()) || mousePosition.y < 0 ||
        mousePosition.y > static_cast<float>(GetScreenHeight()) || (mousePosition.x < 350 && mousePosition.y < 190)) {
        return;
    }
    if (!currentPoint) {
        // first point on press
        handleLeftMousePressFirstPoint(points, mousePosition, currentPoint, roadWidth);
    } else {
        // next point on press
        handleLeftMousePressNextPoint(points, mousePosition, currentPoint, roadWidth);
    }
}

// Handles user input for creating entrances, exits, and road points.
void handleInput(Points &points, PointPtr &currentPoint, std::array<bool, exitColors.size()> &colorsUsed, float roadWidth) {
    Vector2 mousePosition = GetMousePosition();

    if (IsKeyPressed(KEY_S)) {
        auto entrance = handleKeyS(mousePosition);
        points.push_back(entrance);
    } else if (IsKeyPressed(KEY_E)) {
        handleKeyE(points, mousePosition, colorsUsed);
    } else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        handleLeftMousePress(points, mousePosition, currentPoint, roadWidth);
    } else {
        currentPoint.reset();
    }
}

// Draws a road segment between two points.
void drawRoadSegment(const Vector2 &a, const Vector2 &b, float lineWidth) {

    DrawLineEx(a, b, lineWidth, BLACK);

    Vector2 midPoint = {(a.x + b.x) / 2.0f, (a.y + b.y) / 2.0f};
    Vector2 direction = Vector2Subtract(b, a);
    direction = Vector2Normalize(direction);

    float triangleSize = 5.0f;
    Vector2 offset = Vector2Scale(direction, triangleSize / 2.0f);
    Vector2 topPoint = Vector2Add(midPoint, offset);

    Vector2 perpendicular = {-direction.y, direction.x};
    Vector2 bottomLeftPoint = Vector2Subtract(midPoint, offset);
    bottomLeftPoint = Vector2Add(bottomLeftPoint, Vector2Scale(perpendicular, triangleSize / 2.0f));

    Vector2 bottomRightPoint = Vector2Subtract(midPoint, offset);
    bottomRightPoint = Vector2Subtract(bottomRightPoint, Vector2Scale(perpendicular, triangleSize / 2.0f));

    DrawTriangle(bottomLeftPoint, topPoint, bottomRightPoint, GRAY);
}

// Draws a point (entrance, exit, or regular point) on the screen.
void drawPoint(const PointPtr &p, double pointRadius) {
    if (p->isEntrance) {
        DrawCircleV(p->position, static_cast<float>(pointRadius), DARKGRAY);
    } else if (p->isExit) {
        DrawCircleV(p->position, static_cast<float>(pointRadius), p->color);
    } else {
        DrawCircleV(p->position, static_cast<float>(pointRadius) / 3, RED);
    }
}

//
void drawRoads(const Points &points, double pointRadius, float lineWidth) {
    for (const auto &point: points) {
        drawPoint(point, pointRadius);
        for (const auto &connectedPoint: point->connectedPoints) {
            if (point != nullptr && connectedPoint != nullptr) {
                drawRoadSegment(point->position, connectedPoint->position, lineWidth);
            }
        }
    }
}

// The main game loop that initializes the window, handles input, updates car positions, and draws roads and cars.
int main() {
    SetTargetFPS(60);
    InitWindow(1200, 1000, "Highway Game");

    Points points;
    PointPtr currentPoint;
    Cars cars;
    std::array<bool, exitColors.size()> colorsUsed = {false};
    float spawnTimer = 0;

    Settings settings;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        // check if any points are null
        handleInput(points, currentPoint, colorsUsed, settings.roadWidth);
        updateCars(cars, points, deltaTime, settings, spawnTimer);

        BeginDrawing();

        ClearBackground(LIGHTGRAY);
        drawRoads(points, settings.pointRadius, settings.lineWidth);
        drawCars(cars, settings.carLength);
        settings = settingsUI(settings, [&points, &cars, &colorsUsed]() {
            points.clear();
            cars.clear();
            colorsUsed = {false};
        });
        // Draw raygui components
        EndDrawing();
    }

    CloseWindow();
    return 0;
}