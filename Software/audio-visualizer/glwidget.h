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
    void resetState();

signals:
    void bufferChanged(QList<qreal> &buffer);
    void spectrumChanged(FrequencySpectrum &spectrum);
    void spectrumChanged(qint64 position, qint64 length, const FrequencySpectrum &spectrum);

public slots:
    void handleBufferChanged(QList<qreal> &buffer);
    void handleSpectrumChanged(FrequencySpectrum &spectrum);
    void handleSpectrumChanged(qint64 position, qint64 length, const FrequencySpectrum &spectrum);

    // QOpenGLWidget interface
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    //QSurfaceFormat m_format;

    GLScene *currentScene;
    bool busy = false;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};

#endif // GLWIDGET_H
