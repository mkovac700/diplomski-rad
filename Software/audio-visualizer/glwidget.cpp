#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{

}

GLWidget::~GLWidget()
{

}

void GLWidget::setBuffer(QList<QPointF> buffer)
{
    m_buffer = buffer;

    update();

    //qDebug() << "Buffer received! First value: (" << m_buffer.first().x() << ", " << m_buffer.first().y() << ")";
}

void GLWidget::initializeGL()
{

    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width(), -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1, 1, 1);
    glBegin(GL_LINE_STRIP);

    float xScale = 1;

    if(!m_buffer.empty()) xScale = width() / static_cast<float>(m_buffer.size());

    for(int i = 0; i < m_buffer.size(); i++){
        auto x = m_buffer[i].x() * xScale;
        auto y = m_buffer[i].y();
        glVertex2f(x, y);
    }

    glEnd();
    glFlush();
}
