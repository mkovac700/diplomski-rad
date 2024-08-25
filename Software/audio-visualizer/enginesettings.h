#ifndef ENGINESETTINGS_H
#define ENGINESETTINGS_H

#include <QMutex>
#include <QObject>
#include <spectrum.h>

/**
 * The EngineSettings class
 * @brief Singleton
 * @author Marijan Kovač
 */
class EngineSettings : public QObject
{
    Q_OBJECT
public:
    // Static method to get the singleton instance of the EngineSettings class
    static EngineSettings &instance();

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
    EngineSettings();

    // Disable copy constructor and assignment operator to enforce singleton pattern
    EngineSettings(const EngineSettings &) = delete;
    EngineSettings &operator=(const EngineSettings &) = delete;

    WindowFunction m_windowFunction = DefaultWindowFunction;
    int m_fftSize = 1024;
    int m_updateIntervalMs = 10;

    // Mutex to protect access to the settings
    mutable QMutex mutex;
};

#endif // ENGINESETTINGS_H
