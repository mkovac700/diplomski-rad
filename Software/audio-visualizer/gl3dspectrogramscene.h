#ifndef GL3DSPECTROGRAMSCENE_H
#define GL3DSPECTROGRAMSCENE_H

#include <QMatrix4x4>
#include <QMutex>
#include <QMutexLocker>
#include <QTimer>
#include "glscene.h"
#include "glwidget.h"
#include <frequencyspectrum.h>
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
    void spectrumChanged(qint64 position, qint64 length, const FrequencySpectrum &spectrum) override;

private:
    FrequencySpectrum m_spectrum;

    QString name = "3D Spectrogram";

private:
    float m_rotationX;
    float m_rotationY;
    float m_rotationZ;
    float m_positionX;
    float m_positionY;
    float m_distance;
    QPoint m_lastMousePosition;

    const int m_numLines = 50;
    const int m_numPoints = SpectrumLengthSamples / 2 + 1;

    const float m_spacingX = 0.1f;  //0.02f
    const float m_spacingZ = 0.3f;  //0.2f
    const float m_maxHeight = 0.5f; //0.5f

    std::vector<std::vector<qreal>> m_peaks;

    QMatrix4x4 m_projectionMatrix;

    QTimer timer;

    QMutex m_mutex;

    bool initialized = false;

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
