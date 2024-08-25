#include "settingsmanager.h"

SettingsManager::SettingsManager() {}

SettingsManager &SettingsManager::instance()
{
    static SettingsManager instance;
    return instance;
}

EngineSettings &SettingsManager::engineSettings()
{
    return EngineSettings::instance();
}

// GraphicsSettings& SettingsManager::graphicsSettings() {
//     return GraphicsSettings::instance();
// }
