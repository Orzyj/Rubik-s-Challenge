#ifndef MOUSEHANDLE_H
#define MOUSEHANDLE_H

#include <QObject>
#include <QMouseEvent>
#include "openglwidget.h"

class MouseHandle : public QObject
{
    Q_OBJECT
public:
    explicit MouseHandle(QObject *parent = nullptr);
    ~MouseHandle();

    void setReversalAxisX(bool newReversalAxisX);
    void setReversalAxisY(bool newReversalAxisY);

public slots:
    void onMousePressed(const QRect& geometry, const QPoint& mousePosition, OpenGLWidget& widget);
    void onMouseRelesed();
    void onMouseMoved(const QPoint& newPosition, OpenGLWidget& widget);

private:
    const float ROTATE_SPEED = 0.3f;
    const int m_sensitivityThreshold {3};
    bool m_reversalAxisX {false};
    bool m_reversalAxisY {false};


    bool m_isInWindowFocus {false};
    QPoint m_basePoint;
};

#endif // MOUSEHANDLE_H
