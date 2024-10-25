#ifndef KEYBOARDHANDLE_H
#define KEYBOARDHANDLE_H

#include <QObject>
#include <QKeyEvent>
#include "openglwidget.h"

class KeyboardHandle : public QObject
{
    Q_OBJECT
public:
    explicit KeyboardHandle(QObject *parent = nullptr);
    ~KeyboardHandle();

public slots:
    void keyPressed(const int& key, const Qt::KeyboardModifiers& modifier, OpenGLWidget&  widget);
    void keyRelesed(const int& key);

private:
    const int STEP { 1 };
    const float ROTATE { 6.f };


protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
};

#endif // KEYBOARDHANDLE_H
