#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->m_OpenGLWidget = new OpenGLWidget(this);

    ui->frameOpenGL->layout()->addWidget(m_OpenGLWidget);
}

MainWindow::~MainWindow()
{
    if(m_OpenGLWidget != nullptr)
        delete m_OpenGLWidget;

    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->type() == QEvent::KeyPress) {
        const int key = event->key();
        const Qt::KeyboardModifiers modifier = event->modifiers();
        const float rotate = 6.f;

        if(modifier & Qt::ControlModifier) {
            if(key == Qt::Key_Left)     m_OpenGLWidget->onRotateAngelY(rotate);
            if(key == Qt::Key_Right)    m_OpenGLWidget->onRotateAngelY(-rotate);
            if(key == Qt::Key_Up)       m_OpenGLWidget->onRotateAngelX(-rotate);
            if(key == Qt::Key_Down)     m_OpenGLWidget->onRotateAngelX(rotate);
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QRect geometry = m_OpenGLWidget->geometry();
    QPoint mousePosition = event->pos();
    int openGLHeight = m_OpenGLWidget->height();
    int openGLWidth = m_OpenGLWidget->width();

    if(event->button() == Qt::LeftButton &&
    ((mousePosition.x() > geometry.x()) &&
    (mousePosition.x() < geometry.x() + openGLWidth)) &&
    ((mousePosition.y() > geometry.y()) &&
    (mousePosition.y() < geometry.y() + openGLHeight))) {
        m_isInWindowFocus = true;
        m_basePoint = mousePosition;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isInWindowFocus = false;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isInWindowFocus) {
        QPoint newPosition = event->pos();
        const float rotateSpeed = 0.3f;

        int dx = newPosition.x() - m_basePoint.x();
        int dy = newPosition.y() - m_basePoint.y();

        if (abs(dx) > sensitivityThreshold || abs(dy) > sensitivityThreshold) {

            float angleX = rotateSpeed * dy;
            float angleY = rotateSpeed * dx;

            m_OpenGLWidget->onRotateAngelX(angleX);
            m_OpenGLWidget->onRotateAngelY(angleY);

            m_basePoint = newPosition;
        }
    }
}


