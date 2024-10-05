#ifndef CUBE_H
#define CUBE_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QOpenGLFunctions>

class Cube : protected QOpenGLFunctions
{
public:
    Cube(QVector3D position, int x, int y, int z);
    ~Cube();
    void draw();
    QVector3D position;
    QMatrix4x4 modelMatrix;

    int id_x, id_y, id_z;
};

#endif // CUBE_H
