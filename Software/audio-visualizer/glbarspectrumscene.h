#ifndef GLBARSPECTRUMSCENE_H
#define GLBARSPECTRUMSCENE_H

#include "glwidget.h"
#include <frequencyspectrum.h>
#include <glscene.h>

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
    void spectrumChanged(qint64 position, qint64 length, const FrequencySpectrum &spectrum) override;

    // GLScene interface
public:
    QString getName() const override { return name; };

private:
    FrequencySpectrum m_spectrum;

    QString name = "Bar Spectrum";

    // GLScene interface
public:
    void reinitialize() override;
};

#endif // GLBARSPECTRUMSCENE_H
