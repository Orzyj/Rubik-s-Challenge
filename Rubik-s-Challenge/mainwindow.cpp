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

    //rotating camera
    if(modifier & Qt::ControlModifier) {
        switch(key) {
        case Qt::Key_Left:
            qDebug() << "lewo";
            break;
        case Qt::Key_Right:
            break;
        case Qt::Key_Up:
            break;
        case Qt::Key_Down:
            break;
        }
    }



    QMainWindow::keyPressEvent(event);
}
