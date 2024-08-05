#ifndef GLWAVEFORMSCENE_H
#define GLWAVEFORMSCENE_H

#include "glwidget.h"
#include <glscene.h>

class GLWaveformScene : public GLScene
{
    Q_OBJECT
public:
    explicit GLWaveformScene(GLWidget *glWidget)
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
    void spectrumChanged(qint64 position, qint64 length, const FrequencySpectrum &spectrum) override;

private:
    QList<qreal> m_buffer;

    // GLScene interface
public:
    QString getName() const override { return name; };

private:
    QString name = "Waveform";

    // GLScene interface
public:
    void reinitialize() override;
};

#endif // GLWAVEFORMSCENE_H
