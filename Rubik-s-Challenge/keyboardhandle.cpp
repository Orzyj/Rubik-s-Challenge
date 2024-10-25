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
        if(key == Qt::Key_Left)     widget.rotateRowOrColumn(0.0f, 'x', 90.0f);
        if(key == Qt::Key_Right)    widget.rotateRowOrColumn(0.0f, 'y', 90.0f);
        if(key == Qt::Key_Up)       widget.rotateRowOrColumn(0.0f, 'y', 90.0f);
        if(key == Qt::Key_Down)     widget.rotateRowOrColumn(0.0f, 'z', 90.0f);
    } else if(modifier & Qt::ControlModifier) {
        if(key == Qt::Key_Left)     widget.onRotateAngelY(ROTATE);
        if(key == Qt::Key_Right)    widget.onRotateAngelY(-ROTATE);
        if(key == Qt::Key_Up)       widget.onRotateAngelX(-ROTATE);
        if(key == Qt::Key_Down)     widget.onRotateAngelX(ROTATE);
    }
}

void KeyboardHandle::keyRelesed(const int &key)
{
    if (key == Qt::Key_Space) {
        spacePressed = false;
    }
}

bool KeyboardHandle::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        keyPressed(keyEvent->key(), keyEvent->modifiers(), *dynamic_cast<OpenGLWidget*>(obj));
        return true;
    } else if (event->type() == QEvent::KeyRelease) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        keyRelesed(keyEvent->key());
        return true;
    }
    return QObject::eventFilter(obj, event);
}


