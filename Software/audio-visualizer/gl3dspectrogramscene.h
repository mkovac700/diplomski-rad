#ifndef GL3DSPECTROGRAMSCENE_H
#define GL3DSPECTROGRAMSCENE_H

#include <QMatrix4x4>
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
    float m_distance;
    QPoint m_lastMousePosition;

    int m_numLines;
    int m_numPoints;

    const float m_spacing = 0.1f;   //0.2f
    const float m_maxHeight = 0.5f; //0.5f

    std::vector<std::vector<qreal>> m_peaks;

    QMatrix4x4 m_projectionMatrix;

    QTimer timer;

private:
    void updatePeaks();

    // GLScene interface
public:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};

#endif // GL3DSPECTROGRAMSCENE_H
