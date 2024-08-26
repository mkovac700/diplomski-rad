#ifndef GRAPHICSSETTINGS_H
#define GRAPHICSSETTINGS_H

#include <QMutex>
#include <QObject>

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

    // Mutex to protect access to the settings
    mutable QMutex mutex;
};

#endif // GRAPHICSSETTINGS_H
