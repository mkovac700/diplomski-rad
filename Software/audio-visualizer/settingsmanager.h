#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <enginesettings.h>

class SettingsManager
{
public:
    static SettingsManager &instance();

    EngineSettings &engineSettings();
    //GraphicsSettings& graphicsSettings();

private:
    SettingsManager();
    SettingsManager(const SettingsManager &) = delete;
    SettingsManager &operator=(const SettingsManager &) = delete;
};

#endif // SETTINGSMANAGER_H
