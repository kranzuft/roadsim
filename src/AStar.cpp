// A* pathfinding implementation

#include "AStar.h"
#include "model.h"
#include <memory>
#include <array>
#include <unordered_map>
#include <queue>
#include "utility.h"

// A* heuristic function that calculates the distance between two points.
double heuristic(const PointPtr &a, const PointPtr &b) {
    return distance(a->position, b->position);
}

// Reconstructs the path from the A* search
Points reconstructPath(std::unordered_map<PointPtr, PointPtr> &cameFrom, PointPtr current) {
    Points path;
    while (cameFrom.find(current) != cameFrom.end()) {
        path.push_back(current);
        current = cameFrom[current];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

// A* search function to find the shortest path between two points.
Points aStar(const PointPtr &start, const PointPtr &goal,
                                          const Points &points) {
    std::unordered_map<PointPtr, double> gScore;
    std::unordered_map<PointPtr, double> fScore;
    std::unordered_map<PointPtr, PointPtr> cameFrom;

    auto cmp = [&](const PointPtr &a, const PointPtr &b) {
        return fScore[a] > fScore[b];
    };

    std::priority_queue<PointPtr, Points, decltype(cmp)> openSet(cmp);

    for (const auto &point: points) {
        gScore[point] = INFINITY;
        fScore[point] = INFINITY;
    }

    gScore[start] = 0;
    fScore[start] = static_cast<float>(heuristic(start, goal));
    openSet.push(start);

    while (!openSet.empty()) {
        PointPtr current = openSet.top();
        openSet.pop();

        if (current == goal) {
            return reconstructPath(cameFrom, current);
        }

        for (const auto &neighbor: current->connectedPoints) {
            auto tentative_gScore = gScore[current] + distance(current->position, neighbor->position);

            if (tentative_gScore < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_gScore;
                fScore[neighbor] = gScore[neighbor] + heuristic(neighbor, goal);
                openSet.push(neighbor);
            }
        }
    }

    return {}; // No path found
}
