//
// Created by Aden Prior on 19/4/2023.
//

#ifndef ROADS_UI_H
#define ROADS_UI_H

#include <functional>

struct Settings {
    float roadWidth = 10.0f;
    float pointRadius = 5.0f;
    float lineWidth = 1.0f;
    float carLength = 4.0f;
    float carSpeed = 100.0f; // Added missing carSpeed
    float carSpawnInterval = 1.0f; // In seconds, renamed to carSpawnInterval
};

Settings settingsUI(Settings settings, const std::function<void()> &onPressReset);

#endif //ROADS_UI_H
