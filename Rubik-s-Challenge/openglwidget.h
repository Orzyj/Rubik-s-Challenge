#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QTimer>
#include <QSurfaceFormat>
#include <QTimer>
#include <QPainter>
#include "cube.h"

#define DEBUG 1


class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:

    OpenGLWidget(QWidget* parent  = nullptr);
    ~OpenGLWidget();

    void onRotateAngelX(float x);
    void onRotateAngelY(float y);

    float zoomFactorial() const;
    void setZoomFactorial(float newZoomFactorial);
    void rotateRowOrColumn(float positionValue, char axis, float angle);
    void changeSelectedOption();

public slots:
    void onZoomChanged();
    void onNewGame();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

signals:
    void axisXCorrdinatesChanged(float x);
    void axisYCorrdinatesChanged(float y);

private:
    const int m_samples { 6 };
    float m_zoomFactorial { -15.f };
    QSurfaceFormat m_formatAliasing;
    QTimer* m_rotationTimer {nullptr};
    QPainter* m_painter {nullptr};
    unsigned int moves {0};

    //for animation rotating
    float m_targetRotationAngle { 0.f };
    float m_currentRotationAngle { 0.f };
    float m_rotationIncrement { 2.0f };
    QVector<Cube*> m_rotatingCubes;
    char m_rotationAxis;
    float m_rotationPositionValue;
    bool m_isAnimationRunning {false};

    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_modelViewMatrix;
    QVector<Cube*> m_cubes;

    float m_rotateAngelCubeX { 45.f };
    float m_rotateAngelCubeY { 45.f };

    void rotate(float& value, float op_value);
    QVector<Cube*> selectRowOrColumn(float positionValue, char axi);
    void generateCubes();
    void startSmoothRotation(float positionValue, char axis, float angle);
    void rotateCubesSmoothly();

    unsigned short m_selectedOption = 0;
    std::vector<std::vector<short>> m_possibleMoves {
        {0, 1, 2, 3, 4, 5, 6, 7, 8},
        {9, 10, 11, 12, 13, 14, 15, 16, 17},
        {18, 19, 20, 21, 22, 23, 24, 25, 26},

        {0, 1, 2, 9, 10, 11, 18, 19, 20},
        {3, 4, 5, 12, 13, 14, 21, 22, 23},
        {6, 7, 8, 15, 16, 17, 24, 25, 26},

        {2, 11, 20, 5, 14, 23, 8, 17, 26},
        {1, 10, 19, 4, 13, 22, 7, 16, 25},
        {0, 9, 18, 3, 12, 21, 6, 15, 24}
    };
};

#endif // OPENGLWIDGET_H
