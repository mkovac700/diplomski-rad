#ifndef GLWIDGET_H
#define GLWIDGET_H

//#include <QWidget>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <frequencyspectrum.h>
#include <glscene.h>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr);
    ~GLWidget();

    void setScene(GLScene *scene);

signals:
    void bufferChanged(QList<qreal> &buffer);
    void spectrumChanged(FrequencySpectrum &spectrum);

public slots:
    void handleBufferChanged(QList<qreal> &buffer);
    void handleSpectrumChanged(FrequencySpectrum &spectrum);

    // QOpenGLWidget interface
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    //QSurfaceFormat m_format;

    GLScene *currentScene;
};

#endif // GLWIDGET_H
