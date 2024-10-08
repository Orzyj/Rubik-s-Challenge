#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QTimer>
#include <QSurfaceFormat>

#include "cube.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    OpenGLWidget(QWidget* parent  = nullptr);
    ~OpenGLWidget();

    void onRotateAngelX(float x);
    void onRotateAngelY(float y);

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

signals:
    void axisXCorrdinatesChanged(float x);
    void axisYCorrdinatesChanged(float y);

private:
    int m_samples = 6;
    QSurfaceFormat m_formatAliasing;

    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_modelViewMatrix;
    QVector<Cube*> m_cubes;

    float m_rotateAngelCubeX { 45.f };
    float m_rotateAngelCubeY { 45.f };
    bool m_isIncreaseX { false };
    bool m_isIncreaseY { false };

    void rotate(float& value, bool& flag, float op_value);


};

#endif // OPENGLWIDGET_H
