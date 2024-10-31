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
    if(modifier & Qt::ControlModifier) {
        if(key == Qt::Key_Left)     widget.onRotateAngelY(ROTATE);
        if(key == Qt::Key_Right)    widget.onRotateAngelY(-ROTATE);
        if(key == Qt::Key_Up)       widget.onRotateAngelX(-ROTATE);
        if(key == Qt::Key_Down)     widget.onRotateAngelX(ROTATE);
    }

    if (key == Qt::Key_Return || key == Qt::Key_Enter) {
        widget.changeSelectedOption();
    }
}

void KeyboardHandle::keyRelesed(const int &key)
{

}

bool KeyboardHandle::eventFilter(QObject* obj, QEvent* event)
{
    OpenGLWidget* widget = dynamic_cast<OpenGLWidget*>(obj);

    if (!widget){
        qDebug() << "Something went wrong";
        return QObject::eventFilter(obj, event);
    }

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        qDebug() << "KeyPress detected:" << keyEvent->key();
        keyPressed(keyEvent->key(), keyEvent->modifiers(), *dynamic_cast<OpenGLWidget*>(obj));
        return true;
    } else if (event->type() == QEvent::KeyRelease) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        qDebug() << "KeyRelease detected:" << keyEvent->key();
        keyRelesed(keyEvent->key());
        return true;
    }
    return QObject::eventFilter(obj, event);
}



