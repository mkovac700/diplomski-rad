#ifndef GLSCENE_H
#define GLSCENE_H

#include <QList>
#include <QMouseEvent>
#include <QObject>
#include <QOpenGLFunctions>
#include <QString>
#include <QWheelEvent>
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
    virtual QString getName() const = 0;
    virtual void initialize() = 0;
    virtual void reinitialize() = 0;
    virtual void resize(int w, int h) = 0;
    virtual void paint() = 0;

    virtual void mousePressEvent(QMouseEvent *event) = 0;
    virtual void mouseMoveEvent(QMouseEvent *event) = 0;
    virtual void wheelEvent(QWheelEvent *event) = 0;

public slots:
    virtual void bufferChanged(QList<qreal> &buffer) = 0;
    virtual void spectrumChanged(FrequencySpectrum &spectrum) = 0;
    virtual void spectrumChanged(qint64 position, qint64 length, const FrequencySpectrum &spectrum)
        = 0;

protected:
    GLWidget *glWidget;
};

#endif // GLSCENE_H
