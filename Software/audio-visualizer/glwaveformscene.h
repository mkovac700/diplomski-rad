#ifndef GLWAVEFORMSCENE_H
#define GLWAVEFORMSCENE_H

#include <glscene.h>

class GLWaveformScene : public GLScene
{
    Q_OBJECT
public:
    GLWaveformScene();
    explicit GLWaveformScene(GLWidget *glWidget)
        : GLScene(glWidget)
    {}
    // QTC_TEMP
    // GLScene interface
public:
    void initialize() override;
    // void update() override;
    void paint() override;
    void resize(int w, int h) override;

public slots:
    void bufferChanged(QList<qreal> &buffer) override;
    void spectrumChanged(FrequencySpectrum &spectrum) override;

private:
    QList<qreal> m_buffer;
};

#endif // GLWAVEFORMSCENE_H
