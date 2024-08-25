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
    void setLogScale(bool logScale);
    void setLogFactor(qreal logFactor);

    // Thread-safe getters for individual settings
    bool logScale() const;
    qreal logFactor() const;

private:
    // Private constructor to ensure singleton pattern
    GraphicsSettings();

    // Disable copy constructor and assignment operator to enforce singleton pattern
    GraphicsSettings(const GraphicsSettings &) = delete;
    GraphicsSettings &operator=(const GraphicsSettings &) = delete;

    // 3D scene
    bool m_logScale = false;
    qreal m_logFactor = 1;

    // Mutex to protect access to the settings
    mutable QMutex mutex;
};

#endif // GRAPHICSSETTINGS_H
