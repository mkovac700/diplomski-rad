#include "graphicssettings.h"

// Static method to return the singleton instance of the GraphicsSettings class
GraphicsSettings &GraphicsSettings::instance()
{
    static GraphicsSettings instance; // Singleton instance
    return instance;
}

// Constructor (private to enforce singleton)
GraphicsSettings::GraphicsSettings() {}

int GraphicsSettings::numLines() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_numLines;
}

void GraphicsSettings::setNumLines(int newNumLines)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_numLines != newNumLines) {
        m_numLines = newNumLines;
    }
}

float GraphicsSettings::spacingX() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_spacingX;
}

void GraphicsSettings::setSpacingX(float newSpacingX)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_spacingX != newSpacingX) {
        m_spacingX = newSpacingX;
    }
}

float GraphicsSettings::maxHeight() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_maxHeight;
}

void GraphicsSettings::setMaxHeight(float newMaxHeight)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_maxHeight != newMaxHeight) {
        m_maxHeight = newMaxHeight;
    }
}

float GraphicsSettings::spacingZ() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_spacingZ;
}

void GraphicsSettings::setSpacingZ(float newSpacingZ)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_spacingZ != newSpacingZ) {
        m_spacingZ = newSpacingZ;
    }
}

void GraphicsSettings::setIsLogScale(bool logScale)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_isLogScale != logScale) {
        m_isLogScale = logScale;
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

bool GraphicsSettings::isLogScale() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_isLogScale;
}

qreal GraphicsSettings::logFactor() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_logFactor;
}
