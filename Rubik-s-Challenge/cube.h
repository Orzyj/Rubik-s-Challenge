#ifndef CUBE_H
#define CUBE_H

#include <QVector3D>
#include <QMatrix4x4>

class Cube
{
public:
    Cube(QVector3D position);
    ~Cube();
    void draw();
    QVector3D position;
    QMatrix4x4 modelMatrix;
};

#endif // CUBE_H
