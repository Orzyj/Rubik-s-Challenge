#include "cube.h"

Cube::Cube(int id, QVector3D position, int x, int y, int z)
    : m_id(id), position(position), id_x(x), id_y(y), id_z(z){

}

Cube::~Cube()
{

}

void Cube::draw()
{
    glBegin(GL_QUADS);
    const float COLOR = 1.0f - (isSelected ? 0.3f : 0);

    // Front face (z = 1)  Red color
    if (id_z == 2)  glColor3f(COLOR, 0.0f, 0.0f);
    else            glColor3f(0.0f, 0.0f, 0.0f);

    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);

    // Back face (z = -1) Green color
    if (id_z == 0) glColor3f(0.0f, COLOR, 0.0f);
    else           glColor3f(0.0f, 0.0f, 0.0f);

    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);


    // Left face (x = -1) Blue color
    if (id_x == 0) glColor3f(0.0f, 0.0f, COLOR);
    else           glColor3f(0.0f, 0.0f, 0.0f);

    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);

    // Right face (x = 1) Yellow color
    if (id_x == 2) glColor3f(COLOR, COLOR, 0.0f);
    else           glColor3f(0.0f, 0.0f, 0.0f);

    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f,  1.0f, -1.0f);
    glVertex3f(1.0f,  1.0f,  1.0f);
    glVertex3f(1.0f, -1.0f,  1.0f);


    // Top face (y = 1) Cyan color
    if (id_y == 2) glColor3f(0.0f, COLOR, COLOR);
    else           glColor3f(0.0f, 0.0f, 0.0f);

    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);


    // Bottom face (y = -1) Magenta color
    if (id_y == 0)  glColor3f(COLOR, 0.0f, COLOR);
    else            glColor3f(0.0f, 0.0f, 0.0f);

    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);

    glEnd();
}
