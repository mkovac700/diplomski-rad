#include "glbarspectrumscene.h"

GLBarSpectrumScene::GLBarSpectrumScene() {}

void GLBarSpectrumScene::initialize()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, glWidget->width(), 0, glWidget->height(), -1, 1); // Adjusted to start from bottom-left
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLBarSpectrumScene::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1, 1); // Adjusted to start from bottom-left
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLBarSpectrumScene::paint()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1, 1, 1);

    float xScale = glWidget->width() / (float) 20000; //(float) width() / 10000;
    float yScale = glWidget->height() / (float) 1.5f;

    FrequencySpectrum::iterator i = m_spectrum.begin();
    const FrequencySpectrum::iterator end = m_spectrum.end();
    for (; i != end; ++i) {
        const FrequencySpectrum::Element e = *i;

        glBegin(GL_LINE_STRIP);

        // glVertex2f(e.frequency * xScale, 0.0f);              // Start point (x, 0)
        // glVertex2f(e.frequency * xScale, e.amplitude * 100); // End point (x, y) //* 100 //*10

        glVertex2f(e.frequency * xScale, 0.0f * yScale);        // Start point (x, 0)
        glVertex2f(e.frequency * xScale, e.amplitude * yScale); // End point (x, y) //* 100 //*10

        glEnd();
    }

    glFlush();
}

//---------------------
//SLOTS
//---------------------
void GLBarSpectrumScene::bufferChanged(QList<qreal> &buffer)
{
    Q_UNUSED(buffer)
}

void GLBarSpectrumScene::spectrumChanged(FrequencySpectrum &spectrum)
{
    m_spectrum = spectrum;
}

void GLBarSpectrumScene::spectrumChanged(qint64 position,
                                         qint64 length,
                                         const FrequencySpectrum &spectrum)
{
    Q_UNUSED(position)
    Q_UNUSED(length)
    m_spectrum = spectrum;
}
