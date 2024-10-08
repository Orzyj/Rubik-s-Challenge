#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->m_OpenGLWidget = new OpenGLWidget(this);

    baseConfiguration();
}

MainWindow::~MainWindow()
{
    if(m_OpenGLWidget != nullptr)
        delete m_OpenGLWidget;

    delete ui;
}

void MainWindow::baseConfiguration()
{
    ui->frameOpenGL->layout()->addWidget(m_OpenGLWidget);
    m_reversalAxisX = ui->chkboxAxisX->isChecked();
    m_reversalAxisY = ui->chkboxAxisY->isChecked();

    QMainWindow::connect(m_OpenGLWidget, &OpenGLWidget::axisXCorrdinatesChanged, this, &MainWindow::onLabelXTextChanged);
    QMainWindow::connect(m_OpenGLWidget, &OpenGLWidget::axisYCorrdinatesChanged, this, &MainWindow::onLabelYTextChanged);

    QMainWindow::connect(ui->chkboxAxisX, &QCheckBox::stateChanged, this, [this](){
        m_reversalAxisX = ui->chkboxAxisX->isChecked();
    });

    QMainWindow::connect(ui->chkboxAxisY, &QCheckBox::stateChanged, this, [this](){
        m_reversalAxisY = ui->chkboxAxisY->isChecked();
    });
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
        } else {
            if(key == Qt::Key_Left)     m_OpenGLWidget->onColumnSelectedChanged(-1);
            if(key == Qt::Key_Right)    m_OpenGLWidget->onColumnSelectedChanged(1);
            if(key == Qt::Key_Up)       m_OpenGLWidget->onRowSelectedChanged(1);
            if(key == Qt::Key_Down)     m_OpenGLWidget->onRowSelectedChanged(-1);
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

        if (abs(dx) > m_sensitivityThreshold || abs(dy) > m_sensitivityThreshold) {

            float angleX = rotateSpeed * dy * (m_reversalAxisX ? -1 : 1);
            float angleY = rotateSpeed * dx * (m_reversalAxisY ? -1 : 1);

            m_OpenGLWidget->onRotateAngelX(angleX);
            m_OpenGLWidget->onRotateAngelY(angleY);

            m_basePoint = newPosition;
        }
    }
}

void MainWindow::onLabelXTextChanged(const float value)
{
    const QString valueTxt = tr("Wartość osi X: ") + QString::number(value);
    ui->lblXCor->setText(valueTxt);
}

void MainWindow::onLabelYTextChanged(const float value)
{
    const QString valueTxt = tr("Wartość osi Y: ") + QString::number(value);
    ui->lblYCor->setText(valueTxt);
}
