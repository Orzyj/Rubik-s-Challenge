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
    enum ROTATION_BY {
        COLUMNS = 1,
        ROWS = 0
    };


    OpenGLWidget(QWidget* parent  = nullptr);
    ~OpenGLWidget();

    void onRotateAngelX(float x);
    void onRotateAngelY(float y);
    void onRowSelectedChanged(int op_value);
    void onColumnSelectedChanged(int op_value);

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

signals:
    void axisXCorrdinatesChanged(float x);
    void axisYCorrdinatesChanged(float y);

private:
    int m_samples { 6 };
    int m_rowSelected { 1 };
    int m_columnSelected { 1 };
    QSurfaceFormat m_formatAliasing;

    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_modelViewMatrix;
    QVector<Cube*> m_cubes;

    float m_rotateAngelCubeX { 45.f };
    float m_rotateAngelCubeY { 45.f };
    bool m_isIncreaseX { false };
    bool m_isIncreaseY { false };

    void rotate(float& value, float op_value);
    std::vector<Cube*> getCubeInCollection(int id, ROTATION_BY type);


};

#endif // OPENGLWIDGET_H
