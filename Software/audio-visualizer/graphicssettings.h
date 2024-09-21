#ifndef GRAPHICSSETTINGS_H
#define GRAPHICSSETTINGS_H

#include <QMutex>
#include <QObject>
#include <spectrum.h>

/**
 * The GraphicsSettings class
 * @brief Singleton
 * @author Marijan Kovač
 */
class GraphicsSettings : public QObject
{
    Q_OBJECT
public:
    // Static method to get the singleton instance of the GraphicsSettings class
    static GraphicsSettings &instance();

    // Thread-safe setters for individual settings
    void setIsLogScale(bool logScale);
    void setLogFactor(qreal logFactor);

    // Thread-safe getters for individual settings
    bool isLogScale() const;
    qreal logFactor() const;

    float spacingZ() const;
    void setSpacingZ(float newSpacingZ);

    int numLines() const;
    void setNumLines(int newNumLines);
    float spacingX() const;
    void setSpacingX(float newSpacingX);
    float maxHeight() const;
    void setMaxHeight(float newMaxHeight);

    bool applyWindow() const;
    void setApplyWindow(bool newApplyWindow);
    bool drawGrid() const;
    void setDrawGrid(bool newDrawGrid);
    int minFreq() const;
    void setMinFreq(int newMinFreq);
    int maxFreq() const;
    void setMaxFreq(int newMaxFreq);

    int bandWidth() const;
    void setBandWidth(int newBandWidth);

    int gridStepHz() const;
    void setGridStepHz(int newGridStepHz);

    UnitMeasurement spectrogramPowerUnitMeasure() const;
    void setSpectrogramPowerUnitMeasure(UnitMeasurement newSpectrogramPowerUnitMeasure);
    qreal spectrogramYScaleFactor() const;
    void setSpectrogramYScaleFactor(qreal newSpectrogramYScaleFactor);
    UnitMeasurement barPowerUnitMeasure() const;
    void setBarPowerUnitMeasure(UnitMeasurement newBarPowerUnitMeasure);
    qreal barYScaleFactor() const;
    void setBarYScaleFactor(qreal newBarYScaleFactor);

private:
    // Private constructor to ensure singleton pattern
    GraphicsSettings();

    // Disable copy constructor and assignment operator to enforce singleton pattern
    GraphicsSettings(const GraphicsSettings &) = delete;
    GraphicsSettings &operator=(const GraphicsSettings &) = delete;

    // 3D scene
    bool m_isLogScale = false;
    qreal m_logFactor = 1;

    int m_numLines = 275;
    float m_spacingX = 0.02f;
    float m_spacingZ = 2.0f;
    float m_maxHeight = 0.5f;

    UnitMeasurement m_spectrogramPowerUnitMeasure = UnitMeasurement::Magnitude;
    qreal m_spectrogramYScaleFactor = 100.0f;

    //waveform
    bool m_applyWindow = false;

    //bar spectrum
    bool m_drawGrid = false;
    int m_minFreq = 0;
    int m_maxFreq = 24000;
    int m_bandWidth = 1;
    int m_gridStepHz = 100;

    UnitMeasurement m_barPowerUnitMeasure = UnitMeasurement::Magnitude;
    qreal m_barYScaleFactor = 100.0f;

    // Mutex to protect access to the settings
    mutable QMutex mutex;
};

#endif // GRAPHICSSETTINGS_H
