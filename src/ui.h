//
// Created by Aden Prior on 19/4/2023.
//

#ifndef ROADS_UI_H
#define ROADS_UI_H

#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <memory>
#include <array>
#include <unordered_map>
#include "model.h"
#include "AStar.h"
#include "utility.h"

Settings settingsUI(Settings settings, const std::function<void()> &onPressReset);

#endif //ROADS_UI_H
