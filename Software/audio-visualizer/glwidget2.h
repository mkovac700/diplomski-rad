#ifndef GLWIDGET2_H
#define GLWIDGET2_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include <fftw3.h>

#include <signalprocessor.h>

class GLWidget2 : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLWidget2(QWidget *parent = nullptr);
    ~GLWidget2();

    void setBuffer(QList<QPointF> buffer);

    // QOpenGLWidget interface
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QSurfaceFormat m_format;

    QList<QPointF> m_buffer;

    QList<QPointF> dataPoints;

    SignalProcessor signalProcessor;
};

#endif // GLWIDGET2_H
