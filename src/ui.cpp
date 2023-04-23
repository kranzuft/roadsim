//
// Created by Aden Prior on 19/4/2023.
//

#include "ui.h"
#include "raygui.h"

// Draw the UI for the settings
Settings settingsUI(Settings settings, const std::function<void()> &onPressReset) {
    // Draw a rectangle around the sliders
    DrawRectangle(0, 0, 350, 190, Fade(WHITE, 0.5f));
    settings.roadWidth = GuiSliderBar((Rectangle) {130, 10, 200, 20}, "Road Width", nullptr, settings.roadWidth, 1, 50);
    settings.pointRadius = GuiSliderBar((Rectangle) {130, 40, 200, 20}, "Point Radius", nullptr, settings.pointRadius,
                                        1.0f, 30.0f);
    settings.lineWidth = GuiSliderBar((Rectangle) {130, 70, 200, 20}, "Line Width", nullptr, settings.lineWidth, 0.1f,
                                      20.0f);
    settings.carLength = GuiSliderBar((Rectangle) {130, 100, 200, 20}, "Car Length", nullptr, settings.carLength, 1.0f,
                                      20.0f);
    settings.carSpeed = GuiSliderBar((Rectangle) {130, 130, 200, 20}, "Car Speed", nullptr, settings.carSpeed, 1.0f,
                                     500.0f);
    settings.carSpawnInterval = GuiSliderBar((Rectangle) {130, 160, 200, 20}, "Car Spawn Interval", nullptr,
                                             settings.carSpawnInterval, 0.1f, 10.0f);
    bool reset = GuiButton((Rectangle) {10, 190, 100, 30}, "Reset");
    if (reset) {
        onPressReset();
    }

    return settings;
}
