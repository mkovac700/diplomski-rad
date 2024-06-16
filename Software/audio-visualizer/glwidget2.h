#ifndef GLWIDGET2_H
#define GLWIDGET2_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include <fftw3.h>

#define REAL 0
#define IMAG 1
#define N 480
#define Fs 48000.0

class GLWidget2 : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLWidget2(QWidget *parent = nullptr);
    ~GLWidget2();

    void setBuffer(QList<QPointF> buffer);

    void calculateFrequenciesAndMagnitudes(
        fftw_complex *out, int n, double fs, double minFreq, double maxFreq);

    // QOpenGLWidget interface
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QSurfaceFormat m_format;

    QList<QPointF> m_buffer;

    fftw_complex in[N]; //N
    fftw_complex out[N];

    fftw_plan p;

    QList<QPointF> dataPoints;
};

#endif // GLWIDGET2_H
