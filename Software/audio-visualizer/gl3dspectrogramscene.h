#ifndef GL3DSPECTROGRAMSCENE_H
#define GL3DSPECTROGRAMSCENE_H

#include <QMatrix4x4>
#include <QMutex>
#include <QMutexLocker>
#include <QTimer>
#include "glscene.h"
#include "glwidget.h"
#include <enginesettings.h>
#include <frequencyspectrum.h>
#include <graphicssettings.h>
#include <spectrum.h>

class GL3DSpectrogramScene : public GLScene
{
    Q_OBJECT
public:
    explicit GL3DSpectrogramScene(GLWidget *glWidget);

    // GLScene interface
public:
    QString getName() const override { return name; };
    void initialize() override;
    void resize(int w, int h) override;
    void paint() override;
    void reinitialize() override;

public slots:
    void bufferChanged(QList<qreal> &buffer) override;
    void spectrumChanged(FrequencySpectrum &spectrum) override;
    void spectrumChanged(qint64 position,
                         qint64 length,
                         const FrequencySpectrum &spectrum,
                         int inputFrequency) override;

private:
    FrequencySpectrum m_spectrum;
    int m_inputFrequency;
    int m_nquistFrequency;

    QString name = tr("3D Spektrogram");

private:
    float m_rotationX;
    float m_rotationY;
    float m_rotationZ;
    float m_positionX;
    float m_positionY;
    float m_distance;
    QPoint m_lastMousePosition;

    int m_numLines = 275;
    int m_numPoints; //Settings::instance().fftSize() / 2; //SpectrumLengthSamples / 2 + 1

    float m_spacingX = 0.02f; //0.02f
    float m_spacingZ = 2.0f;  //0.2f
    float m_maxHeight = 0.5f; //0.5f

    std::vector<std::vector<qreal>> m_peaks;
    std::vector<std::vector<qreal>> m_freqs;

    QMatrix4x4 m_projectionMatrix;

    QTimer timer;

    QMutex m_mutex;

    bool initialized = false;

    bool logarithm = true;

    int m_centerFrequency;

    qreal m_logFactor;

    UnitMeasurement m_spectrogramPowerUnitMeasure = UnitMeasurement::Magnitude;
    qreal m_spectrogramYScaleFactor = 1.0f;

private:
    void updatePeaks();

    // GLScene interface
public:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    void HSVtoRGB(float H, float S, float V, float &r, float &g, float &b);
};

#endif // GL3DSPECTROGRAMSCENE_H
