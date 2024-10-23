#ifndef CUBE_H
#define CUBE_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QOpenGLFunctions>

class Cube : protected QOpenGLFunctions
{
public:
    Cube(int id, QVector3D position, int x, int y, int z);
    ~Cube();
    void draw();
    QVector3D position;
    QVector3D initialPosition;
    QMatrix4x4 transformMatrix;

    bool isSelected {false};

    int m_id, id_x, id_y, id_z;
};

#endif // CUBE_H
