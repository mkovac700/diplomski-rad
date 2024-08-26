#include "graphicssettings.h"

// Static method to return the singleton instance of the GraphicsSettings class
GraphicsSettings &GraphicsSettings::instance()
{
    static GraphicsSettings instance; // Singleton instance
    return instance;
}

// Constructor (private to enforce singleton)
GraphicsSettings::GraphicsSettings() {}

void GraphicsSettings::setLogScale(bool logScale)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_logScale != logScale) {
        m_logScale = logScale;
        //emit
    }
}

void GraphicsSettings::setLogFactor(qreal logFactor)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_logFactor != logFactor) {
        m_logFactor = logFactor;
        //emit
    }
}

bool GraphicsSettings::logScale() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_logScale;
}

qreal GraphicsSettings::logFactor() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_logFactor;
}
