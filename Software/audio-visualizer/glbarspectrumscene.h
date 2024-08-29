#ifndef GLBARSPECTRUMSCENE_H
#define GLBARSPECTRUMSCENE_H

#include "glwidget.h"
#include <enginesettings.h>
#include <frequencyspectrum.h>
#include <glscene.h>
#include <graphicssettings.h>

class GLBarSpectrumScene : public GLScene
{
    Q_OBJECT
public:
    explicit GLBarSpectrumScene(GLWidget *glWidget)
        : GLScene(glWidget)
    {}

    // GLScene interface
public:
    void initialize() override;
    void paint() override;
    void resize(int w, int h) override;

public slots:
    void bufferChanged(QList<qreal> &buffer) override;
    void spectrumChanged(FrequencySpectrum &spectrum) override;
    void spectrumChanged(qint64 position,
                         qint64 length,
                         const FrequencySpectrum &spectrum,
                         int inputFrequency) override;

    // GLScene interface
public:
    QString getName() const override { return name; };

private:
    FrequencySpectrum m_spectrum;
    int m_inputFrequency;

    qreal m_highFreq = 3000;
    qreal m_lowFreq = 0;
    qint64 m_numBands; //needs to be calculated based on sample rate and fft size and bounds
    qreal m_bandWidth; //calculated by (high freq - low freq) / num bands

    qint64 m_fftSize;

    QString name = "Bar Spectrum";

    // GLScene interface
public:
    void reinitialize() override;

    // GLScene interface
public:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};

#endif // GLBARSPECTRUMSCENE_H
