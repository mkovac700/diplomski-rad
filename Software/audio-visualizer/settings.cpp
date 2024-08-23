#include "settings.h"

// Static method to return the singleton instance of the Settings class
Settings &Settings::instance()
{
    static Settings instance; // Singleton instance
    return instance;
}

// Constructor (private to enforce singleton)
Settings::Settings() {}

void Settings::setWindowFunction(WindowFunction windowFunction)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_windowFunction != windowFunction) {
        m_windowFunction = windowFunction;
        //emit
    }
}

void Settings::setFFTSize(int fftSize)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_fftSize != fftSize) {
        m_fftSize = fftSize;
        //emit
    }
}

void Settings::setUpdateIntervalMs(int intervalMs)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_updateIntervalMs != intervalMs) {
        m_updateIntervalMs = intervalMs;
        //emit
    }
}

WindowFunction Settings::windowFunction() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_windowFunction;
}

int Settings::fftSize() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_fftSize;
}

int Settings::updateIntervalMs() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_updateIntervalMs;
}
