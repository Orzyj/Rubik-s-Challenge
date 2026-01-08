#include "openglwidget.h"
#include <QDebug>
#include <QtMath>

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent) {

    m_formatAliasing.setSamples(m_samples);
    QSurfaceFormat::setDefaultFormat(m_formatAliasing);

    m_rotationTimer = new QTimer(this);
    m_painter = new QPainter(this);
    m_movesStack.clear();

    generateCubes();
}

OpenGLWidget::~OpenGLWidget()
{
    for(Cube* cube : m_cubes) {
        delete cube;
    }
    m_cubes.clear();

    if(m_rotationTimer != nullptr) delete m_rotationTimer;
    if(m_painter != nullptr) delete m_painter;
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

    if (!m_isAnimationRunning) {
        selectRowOrColumn();
    }

    for (Cube* cube : m_cubes) {
        QMatrix4x4 cubeModelMatrix = groupModelMatrix * cube->transformMatrix;
        cubeModelMatrix.translate(cube->position);

        glLoadMatrixf(cubeModelMatrix.constData());
        cube->draw();
    }

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    m_painter->begin(this);
    m_painter->setPen(Qt::yellow);
    m_painter->setFont(QFont("Helvetica", 12));
    m_painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    if(m_cubes.isEmpty()) {
        m_painter->drawText(10, 40, "BŁĄD: Brak kostek!");
    } else {
        m_painter->drawText(10, 20, "Liczba wykonanych ruchow: " + QString::number(moves));
    }

    m_painter->end();
    glPopAttrib();
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
    moves = 0;
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

QVector<Cube *> OpenGLWidget::selectRowOrColumn()
{
    QVector<Cube*> selectedCubes;
    const float EPSILON = 0.8f;

    float targetVal = 0.0f;
    char axis = ' ';

    int layerIndex = m_selectedOption % 3;

    if (layerIndex == 0) targetVal = -2.1f;
    else if (layerIndex == 1) targetVal = 0.0f;
    else targetVal = 2.1f;

    if (m_selectedOption < 3) axis = 'x';
    else if (m_selectedOption < 6) axis = 'y';
    else axis = 'z';

    for (Cube* cube : m_cubes) {
        QVector3D currentPos = cube->transformMatrix.map(cube->position);

        bool match = false;
        if (axis == 'x') match = qAbs(currentPos.x() - targetVal) < EPSILON;
        else if (axis == 'y') match = qAbs(currentPos.y() - targetVal) < EPSILON;
        else if (axis == 'z') match = qAbs(currentPos.z() - targetVal) < EPSILON;

        if (match) {
            cube->isSelected = true;
            selectedCubes.push_back(cube);
        } else {
            cube->isSelected = false;
        }
    }
    return selectedCubes;
}

void OpenGLWidget::generateCubes()
{
    for(auto c : m_cubes) delete c;
    m_cubes.clear();

    m_selectedOption = 0;
    unsigned int id = 0;

    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z, id++) {
                QVector3D position(x * 2.1f, y * 2.1f, z * 2.1f);
                Cube* cube = new Cube(id, position, x + 1, y + 1, z + 1);

                cube->transformMatrix.setToIdentity();
                m_cubes.append(cube);
            }
        }
    }
    update();
}

void OpenGLWidget::startSmoothRotation(Direction direction, char axis, float angle)
{
    if(m_isAnimationRunning)
        return;

    m_targetRotationAngle = angle;
    m_currentRotationAngle = 0.0f;
    m_rotationAxis = axis;

    m_rotatingCubes = selectRowOrColumn();

    if(m_rotatingCubes.isEmpty()) {
        return;
    }

    connect(m_rotationTimer, &QTimer::timeout, this, &OpenGLWidget::rotateCubesSmoothly);
    m_rotationTimer->start(16);
    m_isAnimationRunning = true;
}

void OpenGLWidget::rotateCubesSmoothly()
{
    if (std::abs(m_currentRotationAngle) >= std::abs(m_targetRotationAngle)) {
        m_rotationTimer->stop();
        disconnect(m_rotationTimer, &QTimer::timeout, this, &OpenGLWidget::rotateCubesSmoothly);
        m_isAnimationRunning = false;

        if (m_isShuffling) executeNextShuffleMove();

        return;
    }

    float angleStep = (m_targetRotationAngle > 0 ? m_rotationIncrement : -m_rotationIncrement);
    m_currentRotationAngle += angleStep;

    QMatrix4x4 rotationMatrix;

    if (m_rotationAxis == 'x') rotationMatrix.rotate(angleStep, 1.0f, 0.0f, 0.0f);
    else if (m_rotationAxis == 'y') rotationMatrix.rotate(angleStep, 0.0f, 1.0f, 0.0f);
    else if (m_rotationAxis == 'z') rotationMatrix.rotate(angleStep, 0.0f, 0.0f, 1.0f);

    for (Cube* cube : m_rotatingCubes) {
        cube->transformMatrix = rotationMatrix * cube->transformMatrix;
    }

    update();
}

void OpenGLWidget::executeNextShuffleMove()
{
    if (m_shuffleQueue.empty()) {
        m_isShuffling = false;
        m_rotationIncrement = m_normalRotationIncrement;
        return;
    }

    TMove nextMove = m_shuffleQueue.front();
    m_shuffleQueue.pop();

    m_selectedOption = nextMove.selectedOption;
   rotateRowOrColumn(nextMove.direction, nextMove.axis, nextMove.angle, false);
}

void OpenGLWidget::onBackButtonClicked()
{
    if(!m_movesStack.empty()) {
        TMove move = m_movesStack.back();

        int currentUserOption = m_selectedOption;
        m_selectedOption = move.selectedOption;

        m_movesStack.pop_back();

        Direction direction = move.direction;
        switch(direction) {
            case Direction::Up:    direction = Direction::Down; break;
            case Direction::Down:  direction = Direction::Up; break;
            case Direction::Left:  direction = Direction::Right; break;
            case Direction::Right: direction = Direction::Left; break;
            case Direction::None:  direction = Direction::None; break;
        }

        rotateRowOrColumn(direction, move.axis, -move.angle, false);
        if(moves > 0) moves--;
        m_selectedOption = currentUserOption;

        emit removedLastElement();
        update();
    }
}

void OpenGLWidget::onShuffleElements(int moves)
{
    shuffle(moves);
}

void OpenGLWidget::rotateRowOrColumn(Direction direction, char axis, float angle, bool saveToHistory)
{
    startSmoothRotation(direction, axis, angle);
    if(saveToHistory) {
        int id = m_movesStack.size() + 1;
        m_movesStack.push_back({id, direction, axis, angle, m_selectedOption});
        emit onAddStep(id, direction, axis);
        moves++;
    }
}

void OpenGLWidget::changeSelectedOption(Direction direction)
{
    if (direction == Direction::None) {
        m_selectedOption++;
        if (m_selectedOption >= 9) m_selectedOption = 0;
        if(m_selectedOption < 3) m_axis = 'x';
        else if(m_selectedOption < 6) m_axis = 'y';
        else m_axis = 'z';

        update();
    } else {
        float angle;
        if(direction == Direction::Up || direction == Direction::Right) angle = 90.f;
        else angle = -90.f;

        char rotationAxis = ' ';
        if(m_selectedOption < 3) rotationAxis = 'x';
        else if(m_selectedOption < 6) rotationAxis = 'y';
        else rotationAxis = 'z';

        rotateRowOrColumn(direction, rotationAxis, angle);
    }
}

void OpenGLWidget::shuffle(int movesCount)
{
    if (m_isAnimationRunning || movesCount <= 0) return;

    m_isShuffling = true;
    m_normalRotationIncrement = m_rotationIncrement;
    m_rotationIncrement = 15.0f;

    for (int i = 0; i < movesCount; ++i) {
        int randomOption = rand() % 9;
        Direction randomDir = (rand() % 2 == 0) ? Direction::Up : Direction::Down;
        float angle = (randomDir == Direction::Up) ? 90.f : -90.f;

        char axis;
        if (randomOption < 3) axis = 'x';
        else if (randomOption < 6) axis = 'y';
        else axis = 'z';

        m_shuffleQueue.push({0, randomDir, axis, angle, (unsigned short)randomOption});
    }

    executeNextShuffleMove();
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
