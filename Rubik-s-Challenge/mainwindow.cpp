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
    const int key = event->key();
    const Qt::KeyboardModifiers modifier = event->modifiers();
    const float rotate = 3.f;

    //rotating camera
    if(modifier & Qt::ControlModifier) {
        switch(key) {
        case Qt::Key_Left:
            m_OpenGLWidget->onRotateAngelY(rotate);
            break;
        case Qt::Key_Right:
            m_OpenGLWidget->onRotateAngelY(-rotate);
            break;
        case Qt::Key_Up:
            m_OpenGLWidget->onRotateAngelX(-rotate);
            break;
        case Qt::Key_Down:
            m_OpenGLWidget->onRotateAngelX(rotate);
            break;
        default:
            QMainWindow::keyPressEvent(event);
        }
    }
}
