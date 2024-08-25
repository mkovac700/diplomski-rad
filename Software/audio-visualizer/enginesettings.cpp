#include "enginesettings.h"

// Static method to return the singleton instance of the EngineSettings class
EngineSettings &EngineSettings::instance()
{
    static EngineSettings instance; // Singleton instance
    return instance;
}

// Constructor (private to enforce singleton)
EngineSettings::EngineSettings() {}

void EngineSettings::setWindowFunction(WindowFunction windowFunction)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_windowFunction != windowFunction) {
        m_windowFunction = windowFunction;
        //emit
    }
}

void EngineSettings::setFFTSize(int fftSize)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_fftSize != fftSize) {
        m_fftSize = fftSize;
        //emit
    }
}

void EngineSettings::setUpdateIntervalMs(int intervalMs)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_updateIntervalMs != intervalMs) {
        m_updateIntervalMs = intervalMs;
        //emit
    }
}

WindowFunction EngineSettings::windowFunction() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_windowFunction;
}

int EngineSettings::fftSize() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_fftSize;
}

int EngineSettings::updateIntervalMs() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_updateIntervalMs;
}
