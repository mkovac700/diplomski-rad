#ifndef GLWIDGET_H
#define GLWIDGET_H

//#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr);
    ~GLWidget();

    void setBuffer(QList<QPointF> buffer);

public slots:
    void bufferChanged(QList<qreal> &buffer);

signals:

    // QOpenGLWidget interface
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QSurfaceFormat m_format;

    QList<QPointF> m_buffer;
    QList<qreal> m_buffer2;
};

#endif // GLWIDGET_H
