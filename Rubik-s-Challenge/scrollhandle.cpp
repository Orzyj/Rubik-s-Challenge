#include "scrollhandle.h"

ScrollHandle::ScrollHandle(QObject *parent)
    : QObject{parent}
{

}

void ScrollHandle::scrollUp(OpenGLWidget& widget){
    float zoomFactiorial = widget.zoomFactorial();
    zoomFactiorial -= 1.f;

    widget.setZoomFactorial(zoomFactiorial);
    emit refresh();
}

void ScrollHandle::scrollDown(OpenGLWidget& widget){
    float zoomFactiorial = widget.zoomFactorial();
    zoomFactiorial += 1.f;

    widget.setZoomFactorial(zoomFactiorial);
    emit refresh();
}
