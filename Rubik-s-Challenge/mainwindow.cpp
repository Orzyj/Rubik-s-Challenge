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
    if (event->button() == Qt::LeftButton) {
        qDebug() << "Left mouse button pressed!";
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        qDebug() << "Left mouse button released!";
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "Mouse moved to: " << event->pos();
}


