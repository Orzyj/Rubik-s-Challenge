#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent) {

    m_formatAliasing.setSamples(m_samples);
    QSurfaceFormat::setDefaultFormat(m_formatAliasing);
    m_rotationTimer = new QTimer(this);

    generateCubes();
}

OpenGLWidget::~OpenGLWidget()
{
    for(int i = 0; i < m_cubes.count(); i++)
        delete m_cubes[i];

    if(m_rotationTimer != nullptr)
        delete m_rotationTimer;
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_MULTISAMPLE);
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
    m_modelViewMatrix.translate(0.f, 0.f, m_zoomFactorial);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m_projectionMatrix.constData());
    glMatrixMode(GL_MODELVIEW);

    QMatrix4x4 groupModelMatrix = m_modelViewMatrix;
    groupModelMatrix.rotate(m_rotateAngelCubeX, 1.f, 0.f, 0.f);
    groupModelMatrix.rotate(m_rotateAngelCubeY, 0.f, 1.f, 0.f);

    for (Cube* cube : m_cubes) {
        QMatrix4x4 cubeModelMatrix = groupModelMatrix * cube->transformMatrix;
        cubeModelMatrix.translate(cube->position);

        glLoadMatrixf(cubeModelMatrix.constData());

        cube->draw();
    }
}

float OpenGLWidget::zoomFactorial() const
{
    return m_zoomFactorial;
}

void OpenGLWidget::setZoomFactorial(float newZoomFactorial)
{
    m_zoomFactorial = newZoomFactorial;
}

void OpenGLWidget::onZoomChanged()
{
    update();
}

void OpenGLWidget::onNewGame()
{
    generateCubes();
    m_rotateAngelCubeX = 45.f;
    m_rotateAngelCubeY = 45.f;
    emit axisXCorrdinatesChanged(m_rotateAngelCubeX);
    emit axisYCorrdinatesChanged(m_rotateAngelCubeY);
    update();
}


void OpenGLWidget::rotate(float& valueAxis, float op_value)
{
    valueAxis += op_value;
    update();
}

QVector<Cube *> OpenGLWidget::selectRowOrColumn(float positionValue, char axis)
{
    QVector<Cube*> selectedCubes;

    auto checkPosition = [](float pos1, float pos2) {
        return qFuzzyCompare(pos1, pos2);
    };

    for(Cube* cube : m_cubes) {
        if ((axis == 'x' && checkPosition(cube->position.x(), positionValue)) ||
            (axis == 'y' && checkPosition(cube->position.y(), positionValue)) ||
            (axis == 'z' && checkPosition(cube->position.z(), positionValue))) {
            selectedCubes.push_back(cube);
        }

    }
    return selectedCubes;
}

void OpenGLWidget::generateCubes()
{
    m_cubes.clear();

    unsigned int id = 0;

    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z, id++) {
                QVector3D position(x * 2.1f, y * 2.1f, z * 2.1f);
                Cube* cube = new Cube(id, position, x + 1, y + 1, z + 1);

                m_cubes.append(cube);
            }
        }
    }
}

void OpenGLWidget::startSmoothRotation(float positionValue, char axis, float angle)
{
    if(m_isAnimationRunning)
        return;

    m_targetRotationAngle = angle;
    m_currentRotationAngle = 0.0f;
    m_rotationAxis = axis;
    m_rotationPositionValue = positionValue;
    m_rotatingCubes = selectRowOrColumn(positionValue, axis);

    connect(m_rotationTimer, &QTimer::timeout, this, &OpenGLWidget::rotateCubesSmoothly);
    m_rotationTimer->start(16); // 1000ms / 16 = ~62fps
    m_isAnimationRunning = true;
}

void OpenGLWidget::rotateCubesSmoothly()
{
    if (std::abs(m_currentRotationAngle) >= std::abs(m_targetRotationAngle)) {
        m_rotationTimer->stop();
        disconnect(m_rotationTimer, &QTimer::timeout, this, &OpenGLWidget::rotateCubesSmoothly);
        m_isAnimationRunning = false;
        return;
    }

    float angleStep = (m_targetRotationAngle > 0 ? m_rotationIncrement : -m_rotationIncrement);
    m_currentRotationAngle += angleStep;

    QMatrix4x4 rotationMatrix;
    if (m_rotationAxis == 'x') {
        rotationMatrix.rotate(angleStep, 1.0f, 0.0f, 0.0f);
    } else if (m_rotationAxis == 'y') {
        rotationMatrix.rotate(angleStep, 0.0f, 1.0f, 0.0f);
    } else if (m_rotationAxis == 'z') {
        rotationMatrix.rotate(angleStep, 0.0f, 0.0f, 1.0f);
    }

    for (Cube* cube : m_rotatingCubes) {
        cube->transformMatrix = rotationMatrix * cube->transformMatrix;
    }

    update();
}

void OpenGLWidget::rotateRowOrColumn(float positionValue, char axis, float angle)
{
    startSmoothRotation(positionValue, axis, angle);
}

void OpenGLWidget::onRotateAngelX(float x)
{
    rotate(m_rotateAngelCubeX, x);
    emit axisXCorrdinatesChanged(m_rotateAngelCubeX);
}

void OpenGLWidget::onRotateAngelY(float y)
{
    rotate(m_rotateAngelCubeY, y);
    emit axisYCorrdinatesChanged(m_rotateAngelCubeY);
}


