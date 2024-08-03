#ifndef GLSCENE_H
#define GLSCENE_H

#include <QList>
#include <QObject>
#include <QOpenGLFunctions>
#include <frequencyspectrum.h>

class GLWidget;

class GLScene : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLScene() {}
    explicit GLScene(GLWidget *glWidget)
        : glWidget(glWidget)
    {}
    virtual ~GLScene() = default;
    virtual void initialize() = 0;
    // virtual void update() = 0;
    virtual void paint() = 0;
    virtual void resize(int w, int h) = 0;
public slots:
    virtual void bufferChanged(QList<qreal> &buffer) = 0;
    virtual void spectrumChanged(FrequencySpectrum &spectrum) = 0;

protected:
    GLWidget *glWidget;
};

#endif // GLSCENE_H
