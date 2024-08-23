#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMutex>
#include <QObject>
#include <spectrum.h>

/**
 * The Settings class
 * @brief Singleton
 * @author Marijan Kovač
 */
class Settings : public QObject
{
    Q_OBJECT
public:
    // Static method to get the singleton instance of the Settings class
    static Settings &instance();

    // Thread-safe setters for individual settings
    void setWindowFunction(WindowFunction windowFunction);
    void setFFTSize(int fftSize);
    void setUpdateIntervalMs(int intervalMs);

    // Thread-safe getters for individual settings
    WindowFunction windowFunction() const;
    int fftSize() const;
    int updateIntervalMs() const;

signals:
    // Signals emitted when individual settings change
    // void windowFunctionChanged(WindowFunction newValue);
    // void fftSizeChanged(int newValue);
    // void updateIntervalMsChanged(int newValue);

private:
    // Private constructor to ensure singleton pattern
    Settings();

    // Disable copy constructor and assignment operator to enforce singleton pattern
    Settings(const Settings &) = delete;
    Settings &operator=(const Settings &) = delete;

    WindowFunction m_windowFunction = DefaultWindowFunction;
    int m_fftSize = 1024;
    int m_updateIntervalMs = 10;

    // Mutex to protect access to the settings
    mutable QMutex mutex;
};

#endif // SETTINGS_H
