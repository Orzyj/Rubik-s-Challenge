#include "mousehandle.h"

MouseHandle::MouseHandle(QObject *parent)
    : QObject{parent}
{}

MouseHandle::~MouseHandle()
{

}

void MouseHandle::onMousePressed(const QRect& geometry, const QPoint& mousePosition, OpenGLWidget& widget)
{
    int openGLHeight = widget.height();
    int openGLWidth = widget.width();

    if(((mousePosition.x() > geometry.x()) &&
    (mousePosition.x() < geometry.x() + openGLWidth)) &&
    ((mousePosition.y() > geometry.y()) &&
    (mousePosition.y() < geometry.y() + openGLHeight))) {
        m_isInWindowFocus = true;
        m_basePoint = mousePosition;
    }
}

void MouseHandle::onMouseRelesed()
{
    m_isInWindowFocus = false;
}

void MouseHandle::onMouseMoved(const QPoint& newPosition, OpenGLWidget& widget)
{
    if (m_isInWindowFocus) {
        int dx = newPosition.x() - m_basePoint.x();
        int dy = newPosition.y() - m_basePoint.y();

        if (abs(dx) > m_sensitivityThreshold || abs(dy) > m_sensitivityThreshold) {

            float angleX = ROTATE_SPEED * dy * (m_reversalAxisX ? -1 : 1);
            float angleY = ROTATE_SPEED * dx * (m_reversalAxisY ? -1 : 1);

            widget.onRotateAngelX(angleX);
            widget.onRotateAngelY(angleY);

            m_basePoint = newPosition;
        }
    }
}

void MouseHandle::setReversalAxisY(bool newReversalAxisY)
{
    m_reversalAxisY = newReversalAxisY;
}

void MouseHandle::setReversalAxisX(bool newReversalAxisX)
{
    m_reversalAxisX = newReversalAxisX;
}
