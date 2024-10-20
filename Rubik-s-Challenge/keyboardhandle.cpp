#include "keyboardhandle.h"

KeyboardHandle::KeyboardHandle(QObject *parent)
    : QObject{parent}
{

}

KeyboardHandle::~KeyboardHandle()
{

}

void KeyboardHandle::keyPressed(const int &key, const Qt::KeyboardModifiers &modifier, OpenGLWidget &widget)
{
    if(key == Qt::Key_Space) spacePressed = true;

    if(spacePressed) {
        if(key == Qt::Key_Left)     widget.onRotateSelectedCubes(ROTATION_BY::LEFT);
        if(key == Qt::Key_Right)    widget.onRotateSelectedCubes(ROTATION_BY::RIGHT);
        if(key == Qt::Key_Up)       widget.onRotateSelectedCubes(ROTATION_BY::UP);
        if(key == Qt::Key_Down)     widget.onRotateSelectedCubes(ROTATION_BY::DOWN);
    } else if(modifier & Qt::ControlModifier) {
        if(key == Qt::Key_Left)     widget.onRotateAngelY(ROTATE);
        if(key == Qt::Key_Right)    widget.onRotateAngelY(-ROTATE);
        if(key == Qt::Key_Up)       widget.onRotateAngelX(-ROTATE);
        if(key == Qt::Key_Down)     widget.onRotateAngelX(ROTATE);
    } else if (modifier & Qt::ShiftModifier) {
        if(key == Qt::Key_Left)     widget.onDeepLevelSelectedChanged(-STEP);
        if(key == Qt::Key_Right)    widget.onDeepLevelSelectedChanged(STEP);
    } else {
        if(key == Qt::Key_Left)     widget.onColumnSelectedChanged(-STEP);
        if(key == Qt::Key_Right)    widget.onColumnSelectedChanged(STEP);
        if(key == Qt::Key_Up)       widget.onRowSelectedChanged(STEP);
        if(key == Qt::Key_Down)     widget.onRowSelectedChanged(-STEP);
    }
}

void KeyboardHandle::keyRelesed(const int &key)
{
    spacePressed = false;

}
