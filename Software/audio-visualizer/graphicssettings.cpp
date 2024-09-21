#include "graphicssettings.h"

// Static method to return the singleton instance of the GraphicsSettings class
GraphicsSettings &GraphicsSettings::instance()
{
    static GraphicsSettings instance; // Singleton instance
    return instance;
}

// Constructor (private to enforce singleton)
GraphicsSettings::GraphicsSettings() {}

UnitMeasurement GraphicsSettings::spectrogramPowerUnitMeasure() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_spectrogramPowerUnitMeasure;
}

void GraphicsSettings::setSpectrogramPowerUnitMeasure(UnitMeasurement newSpectrogramPowerUnitMeasure)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_spectrogramPowerUnitMeasure != newSpectrogramPowerUnitMeasure) {
        m_spectrogramPowerUnitMeasure = newSpectrogramPowerUnitMeasure;
    }
}

qreal GraphicsSettings::spectrogramYScaleFactor() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_spectrogramYScaleFactor;
}

void GraphicsSettings::setSpectrogramYScaleFactor(qreal newSpectrogramYScaleFactor)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_spectrogramYScaleFactor != newSpectrogramYScaleFactor) {
        m_spectrogramYScaleFactor = newSpectrogramYScaleFactor;
    }
}

UnitMeasurement GraphicsSettings::barPowerUnitMeasure() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_barPowerUnitMeasure;
}

void GraphicsSettings::setBarPowerUnitMeasure(UnitMeasurement newBarPowerUnitMeasure)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_barPowerUnitMeasure != newBarPowerUnitMeasure) {
        m_barPowerUnitMeasure = newBarPowerUnitMeasure;
    }
}

qreal GraphicsSettings::barYScaleFactor() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_barYScaleFactor;
}

void GraphicsSettings::setBarYScaleFactor(qreal newBarYScaleFactor)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_barYScaleFactor != newBarYScaleFactor) {
        m_barYScaleFactor = newBarYScaleFactor;
    }
}

int GraphicsSettings::gridStepHz() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_gridStepHz;
}

void GraphicsSettings::setGridStepHz(int newGridStepHz)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_gridStepHz != newGridStepHz) {
        m_gridStepHz = newGridStepHz;
    }
}

int GraphicsSettings::bandWidth() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_bandWidth;
}

void GraphicsSettings::setBandWidth(int newBandWidth)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_bandWidth != newBandWidth) {
        m_bandWidth = newBandWidth;
    }
}

bool GraphicsSettings::applyWindow() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_applyWindow;
}

void GraphicsSettings::setApplyWindow(bool newApplyWindow)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_applyWindow != newApplyWindow) {
        m_applyWindow = newApplyWindow;
    }
}

bool GraphicsSettings::drawGrid() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_drawGrid;
}

void GraphicsSettings::setDrawGrid(bool newDrawGrid)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_drawGrid != newDrawGrid) {
        m_drawGrid = newDrawGrid;
    }
}

int GraphicsSettings::minFreq() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_minFreq;
}

void GraphicsSettings::setMinFreq(int newMinFreq)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_minFreq != newMinFreq) {
        m_minFreq = newMinFreq;
    }
}

int GraphicsSettings::maxFreq() const
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    return m_maxFreq;
}

void GraphicsSettings::setMaxFreq(int newMaxFreq)
{
    QMutexLocker locker(&mutex); // Lock the mutex for thread safety
    if (m_maxFreq != newMaxFreq) {
        m_maxFreq = newMaxFreq;
    }
}

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
