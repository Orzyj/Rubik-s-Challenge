#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent) {

    m_formatAliasing.setSamples(m_samples);
    QSurfaceFormat::setDefaultFormat(m_formatAliasing);
    unsigned int id = 0;

    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z, id++) {
                QVector3D position(x * 2.1f, y * 2.1f, z * 2.1f);
                m_cubes.append(new Cube(id, position, x + 1, y + 1, z + 1));
            }
        }
    }

    for(int i = 0; i < m_cubes.count(); i++) {
        if(m_cubes[i]->id_y == m_rowSelected)
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

void OpenGLWidget::onChangeSelection(int &idColOrRow, int op_value, ROTATION_BY type)
{
    int id = idColOrRow;
    id += op_value;

    if(id >= 2)         id = 2;
    else if(id <= 0)    id = 0;

    idColOrRow = id;

    for(Cube* cube : m_cubes){
        cube->isSelected = false;
    }

    for(Cube* cube : m_cubes) {
        if(cube->id_x == m_columnSelected && type == ROTATION_BY::COLUMNS)
            cube->isSelected = true;
        else if(cube->id_y == m_rowSelected&& type == ROTATION_BY::ROWS)
            cube->isSelected = true;
        else if(cube->id_z == m_deep&& type == ROTATION_BY::DEEP)
            cube->isSelected = true;
    }

    update();
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
    onChangeSelection(m_rowSelected, op_value, ROTATION_BY::ROWS);
    actualSelectedCubes.clear();
    actualSelectedCubes = getCubeInCollection(m_rowSelected, ROTATION_BY::ROWS);
}

void OpenGLWidget::onColumnSelectedChanged(int op_value)
{
    onChangeSelection(m_columnSelected, op_value, ROTATION_BY::COLUMNS);
    actualSelectedCubes.clear();
    actualSelectedCubes = getCubeInCollection(m_columnSelected, ROTATION_BY::COLUMNS);
}

void OpenGLWidget::onDeepLevelSelectedChanged(int op_value)
{
    onChangeSelection(m_deep, op_value, ROTATION_BY::DEEP);
    actualSelectedCubes.clear();
    actualSelectedCubes = getCubeInCollection(m_deep, ROTATION_BY::DEEP);
}

void OpenGLWidget::onRotateSelectedCubes(ROTATION_BY type)
{
    qDebug() << type;
}

std::vector<Cube *> OpenGLWidget::getCubeInCollection(int id, ROTATION_BY type)
{
    std::vector<Cube*> cubes;

    for(Cube* cube : m_cubes) {
        if((type == ROTATION_BY::ROWS && cube->id_y == id) ||
            (type == ROTATION_BY::COLUMNS && cube->id_x == id) ||
            (type == ROTATION_BY::DEEP && cube->id_z == id))
            {
            cubes.push_back(cube);
        }
    }
    return cubes;
}

