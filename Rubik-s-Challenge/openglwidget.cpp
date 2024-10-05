#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent) {
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z) {
                QVector3D position(x * 2.1f, y * 2.1f, z * 2.1f);
                m_cubes.append(new Cube(position));
            }
        }
    }
}

OpenGLWidget::~OpenGLWidget()
{
    for(int i = 0; i < m_cubes.count(); i++)
        delete m_cubes[i];
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void OpenGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(45.0f, float(width) / float(height), 0.1f, 100.0f);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_modelViewMatrix.setToIdentity();
    m_modelViewMatrix.translate(0.f, 0.f, -15.f);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m_projectionMatrix.constData());
    glMatrixMode(GL_MODELVIEW);

    QMatrix4x4 groupModelMatrix = m_modelViewMatrix;
    groupModelMatrix.rotate(m_rotateAngelCubeX, 1.f, 0.f, 0.f);
    groupModelMatrix.rotate(m_rotateAngelCubeY, 0.f, 1.f, 0.f);

    for (Cube* cube : m_cubes) {
        QMatrix4x4 cubeModelMatrix = groupModelMatrix;

        cubeModelMatrix.translate(cube->position);

        glLoadMatrixf(cubeModelMatrix.constData());

        cube->draw();
    }
}

void OpenGLWidget::rotate(float& valueAxis, bool& flag, float op_value)
{
    float max = 360.f, min = -360.f;
    if(flag) {
        valueAxis += op_value;

        if(valueAxis >= max) {
            valueAxis = max;
            flag = false;
        }
    } else {
        valueAxis -= op_value;

        if(valueAxis <= min) {
            valueAxis = min;
            flag = true;
        }
    }
    update();
}

void OpenGLWidget::onRotateAngelX(float x)
{
    qDebug() << "x";
    rotate(m_rotateAngelCubeX, m_isIncreaseX, x);
}

void OpenGLWidget::onRotateAngelY(float y)
{
    qDebug() << "y";
    rotate(m_rotateAngelCubeY, m_isIncreaseY, y);
}
