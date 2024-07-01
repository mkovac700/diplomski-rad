#include "glwidget2.h"

GLWidget2::GLWidget2(QWidget *parent)
    : QOpenGLWidget(parent)
{}

GLWidget2::~GLWidget2() {}

void GLWidget2::setBuffer(QList<QPointF> buffer)
{
    dataPoints.clear();
    dataPoints.append(signalProcessor.process(buffer, SignalProcessor::WindowingMode::Bartlett));

    update();
}

void GLWidget2::initializeGL()
{
    initializeOpenGLFunctions();

    // glClearColor(0, 0, 0, 1);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // glOrtho(0, width(), -1, 1, -1, 1);
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();

    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width(), 0, height(), -1, 1); // Adjusted to start from bottom-left
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget2::resizeGL(int w, int h)
{
    // glViewport(0, 0, w, h);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // glOrtho(0, w, -1, 1, -1, 1);
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1, 1); // Adjusted to start from bottom-left
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget2::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1, 1, 1);
    //glBegin(GL_LINE_STRIP);

    //glLineWidth(10.0f); // Set line width to 2.0 pixels

    //float yScale = 1;

    //yScale = height() / 100;

    // for (int i = 0; i < m_buffer.size(); i++) {
    //     auto x = m_buffer[i].x() * xScale;
    //     auto y = m_buffer[i].y();
    //     glVertex2f(x, y);
    // }

    // glEnd();
    // glFlush();

    float xScale = width() / (float) 20000; //(float) width() / 10000;

    for (const QPointF point : dataPoints) {
        glBegin(GL_LINE_STRIP);

        glVertex2f(point.x() * xScale, 0.0f);           // Start point (x, 0)
        glVertex2f(point.x() * xScale, point.y() * 10); // End point (x, y) //* 100

        glEnd();
    }

    //glEnd();
    glFlush();
}
