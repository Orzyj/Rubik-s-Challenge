#ifndef SCROLLHANDLE_H
#define SCROLLHANDLE_H

#include <QObject>
#include "openglwidget.h"

class ScrollHandle : public QObject
{
    Q_OBJECT
public:
    explicit ScrollHandle(QObject *parent = nullptr);

public slots:
    void scrollUp(OpenGLWidget&  widget);
    void scrollDown(OpenGLWidget&  widget);

signals:
    void refresh();

};

#endif // SCROLLHANDLE_H
