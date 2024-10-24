#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QTimer>
#include <QSurfaceFormat>

#include "cube.h"
#include "rotationTypes.h"

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

public slots:
    void onZoomChanged();

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

    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_modelViewMatrix;
    QVector<Cube*> m_cubes;

    std::vector<Cube*> actualSelectedCubes;

    float m_rotateAngelCubeX { 45.f };
    float m_rotateAngelCubeY { 45.f };

    void rotate(float& value, float op_value);
    QVector<Cube*> selectRowOrColumn(float positionValue, char axi);

};

#endif // OPENGLWIDGET_H
