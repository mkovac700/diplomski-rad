#include "glwidget.h"
#include <QTime>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , currentScene(nullptr)
{

}

GLWidget::~GLWidget()
{

}

void GLWidget::setScene(GLScene *scene)
{
    if (currentScene) {
        // Prekini sve signale povezane s trenutnom scenom
        disconnect(this, nullptr, currentScene, nullptr);
        delete currentScene;
    }
    currentScene = scene;
    if (currentScene) {
        currentScene->initialize();
        connect(this, &GLWidget::bufferChanged, currentScene, &GLScene::bufferChanged);
        connect(this,
                QOverload<qint64, qint64, const FrequencySpectrum &>::of(&GLWidget::spectrumChanged),
                currentScene,
                QOverload<qint64, qint64, const FrequencySpectrum &>::of(&GLScene::spectrumChanged));
    }
}

void GLWidget::handleBufferChanged(QList<qreal> &buffer)
{
    emit bufferChanged(buffer);
    update(); // Zove paintGL()
}

void GLWidget::handleSpectrumChanged(FrequencySpectrum &spectrum)
{
    emit spectrumChanged(spectrum);
    update(); // Zove paintGL()
}

void GLWidget::handleSpectrumChanged(qint64 position,
                                     qint64 length,
                                     const FrequencySpectrum &spectrum)
{
    emit spectrumChanged(position, length, spectrum);
    update(); // Zove paintGL()
}

void GLWidget::initializeGL()
{
    if (currentScene) {
        currentScene->initialize();
    }
}

void GLWidget::resizeGL(int w, int h)
{
    if (currentScene) {
        currentScene->resize(w, h);
    }
}

void GLWidget::paintGL()
{
    if (currentScene) {
        currentScene->paint();
    }
}
