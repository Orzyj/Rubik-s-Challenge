#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent) {

    m_formatAliasing.setSamples(m_samples);
    QSurfaceFormat::setDefaultFormat(m_formatAliasing);

    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z) {
                QVector3D position(x * 2.1f, y * 2.1f, z * 2.1f);
                m_cubes.append(new Cube(position, x + 1, y + 1, z + 1));
            }
        }
    }

    for(int i = 0; i < m_cubes.count(); i++) {
        if(m_cubes[i]->id_y == 1)
            m_cubes[i]->isSelected = true;
        if(m_cubes[i]->id_x == 1)
            m_cubes[i]->isSelected = true;
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

void OpenGLWidget::rotate(float& valueAxis, float op_value)
{
    valueAxis += op_value;
    update();
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

void OpenGLWidget::onRowSelectedChanged(int op_value)
{
    int id_row = m_rowSelected;
    id_row += op_value;

    if(id_row >= 2)
        id_row = 2;
    else if (id_row <= 0)
        id_row = 0;

    m_rowSelected = id_row;
    qDebug() << id_row;

}

void OpenGLWidget::onColumnSelectedChanged(int op_value)
{
    int id_column = m_columnSelected;
    id_column += op_value;

    if(id_column >= 2)
        id_column = 2;
    else if (id_column <= 0)
        id_column = 0;

    m_columnSelected = id_column;
    qDebug() << id_column;
}

std::vector<Cube *> OpenGLWidget::getCubeInCollection(int id, ROTATION_BY type)
{
    std::vector<Cube*> cubes;

    for(Cube* cube : m_cubes) {
        if((type == ROTATION_BY::ROWS && cube->id_y == id) ||
            (type == ROTATION_BY::COLUMNS && cube->id_x == id)) {
            cubes.push_back(cube);
        }
    }
    return cubes;
}

