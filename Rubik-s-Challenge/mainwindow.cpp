#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "contextmenuhandler.h"
#include "historylogger.h"
#include <QHBoxLayout>

#include <dwmapi.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->m_OpenGLWidget = new OpenGLWidget(this);
    this->m_scrollHandle = new ScrollHandle(this);
    this->m_keyboardHandle = new KeyboardHandle(this);
    this->m_mouseHandle = new MouseHandle(this);
    this->m_stepTrackerListWidget = new StepTracker(this);

    m_OpenGLWidget->installEventFilter(m_keyboardHandle);

    baseConfiguration();

    //Enable dark
    BOOL navigationBar = TRUE;
    HWND hwnd = (HWND)this->winId();
    DwmSetWindowAttribute(hwnd, 20, &navigationBar, sizeof(navigationBar));
}

MainWindow::~MainWindow()
{
    if(m_OpenGLWidget != nullptr)
        delete m_OpenGLWidget;

    if(m_scrollHandle != nullptr)
        delete m_scrollHandle;

    if(m_keyboardHandle != nullptr)
        delete m_keyboardHandle;

    if(m_mouseHandle != nullptr)
        delete m_mouseHandle;

    if(m_stepTrackerListWidget != nullptr)
        delete m_stepTrackerListWidget;

    delete ui;
}

void MainWindow::baseConfiguration()
{
    ui->frameOpenGL->layout()->addWidget(m_OpenGLWidget);
    bool reversalAxisX = ui->chkboxAxisX->isChecked();
    bool reversalAxisY = ui->chkboxAxisY->isChecked();

    m_mouseHandle->setReversalAxisX(reversalAxisX);
    m_mouseHandle->setReversalAxisY(reversalAxisY);
    ui->lblXCor->setText("Wartość osi X: "+ QString::number(45.0));
    ui->lblYCor->setText("Wartość osi Y: "+ QString::number(45.0));

    ui->frameListTracker->setLayout(new QHBoxLayout());
    ui->frameListTracker->layout()->addWidget(m_stepTrackerListWidget);

    QMainWindow::connect(this, &MainWindow::scrolledUp, m_scrollHandle, &ScrollHandle::scrollUp);
    QMainWindow::connect(this, &MainWindow::scrolledDown, m_scrollHandle, &ScrollHandle::scrollDown);
    QMainWindow::connect(this, &MainWindow::keyPressed, m_keyboardHandle, &KeyboardHandle::keyPressed);
    QMainWindow::connect(this, &MainWindow::keyRelesed, m_keyboardHandle, &KeyboardHandle::keyRelesed);
    QMainWindow::connect(this, &MainWindow::mousePressed, m_mouseHandle, &MouseHandle::onMousePressed);
    QMainWindow::connect(this, &MainWindow::mouseRelesed, m_mouseHandle, &MouseHandle::onMouseRelesed);
    QMainWindow::connect(this, &MainWindow::mouseMoved, m_mouseHandle, &MouseHandle::onMouseMoved);
    QMainWindow::connect(m_scrollHandle, &ScrollHandle::refresh, m_OpenGLWidget, &OpenGLWidget::onZoomChanged);
    QMainWindow::connect(m_OpenGLWidget, &OpenGLWidget::axisXCorrdinatesChanged, this, &MainWindow::onLabelXTextChanged);
    QMainWindow::connect(m_OpenGLWidget, &OpenGLWidget::axisYCorrdinatesChanged, this, &MainWindow::onLabelYTextChanged);
    QMainWindow::connect(ui->chkboxAxisX,&QCheckBox::stateChanged, this, &MainWindow::onCheckBoxAxisStateChanged);
    QMainWindow::connect(ui->chkboxAxisY,&QCheckBox::stateChanged, this, &MainWindow::onCheckBoxAxisStateChanged);
    QMainWindow::connect(m_OpenGLWidget, &OpenGLWidget::onAddStep, m_stepTrackerListWidget, &StepTracker::onAddItem);
    QMainWindow::connect(ui->btnBackSteps, &QPushButton::clicked, m_OpenGLWidget, &OpenGLWidget::onBackButtonClicked);
    QMainWindow::connect(m_OpenGLWidget, &OpenGLWidget::removedLastElement, m_stepTrackerListWidget, &StepTracker::back);

    QList<QPushButton*> buttonsShuffle = ui->centralwidget->findChildren<QPushButton*>();
    for(int i = 0; i < buttonsShuffle.size(); i++) {
        if(buttonsShuffle[i] && buttonsShuffle[i]->objectName().contains("btnShuffle")) {
            QMainWindow::connect(buttonsShuffle[i], &QPushButton::clicked, this, &MainWindow::onShuffleElements);
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->type() == QEvent::KeyPress) {
        const int key = event->key();
        const Qt::KeyboardModifiers modifier = event->modifiers();

        emit keyPressed(key, modifier, *m_OpenGLWidget);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space) {
        const int key = event->key();

        emit keyRelesed(key);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QRect geometry = m_OpenGLWidget->geometry();
    QPoint mousePosition = event->pos();
    auto button = event->button();

    if(button == Qt::LeftButton)
        emit mousePressed(geometry, mousePosition, *m_OpenGLWidget);
    else if(button == Qt::RightButton)
        onRightButtonMouse(mousePosition);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit mouseRelesed();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint newPosition = event->pos();

    emit mouseMoved(newPosition, *m_OpenGLWidget);
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    int delta = event->angleDelta().y();

    if (delta > 0)          emit scrolledUp(*m_OpenGLWidget);
    else if (delta < 0)     emit scrolledDown(*m_OpenGLWidget);

    QWidget::wheelEvent(event);
}

void MainWindow::onLabelXTextChanged(const float& value)
{
    const QString valueTxt = tr("Wartość osi X: ") + QString::number(value);
    ui->lblXCor->setText(valueTxt);
}

void MainWindow::onLabelYTextChanged(const float& value)
{
    const QString valueTxt = tr("Wartość osi Y: ") + QString::number(value);
    ui->lblYCor->setText(valueTxt);
}

void MainWindow::onCheckBoxAxisStateChanged(const bool &state)
{
    auto _sender = sender();

    if(_sender->objectName() == "chkboxAxisX")
        m_mouseHandle->setReversalAxisX(state);
    else if (_sender->objectName() == "chkboxAxisY")
        m_mouseHandle->setReversalAxisY(state);
}

void MainWindow::onShuffleElements()
{
    QPushButton* _sender = static_cast<QPushButton*>(sender());
    int moves = _sender->text().toInt();
    m_OpenGLWidget->shuffle(moves);
}

void MainWindow::onRightButtonMouse(const QPoint &mousePosition)
{
    QMetaObject::Connection csvConnection;
    QMetaObject::Connection jsonConnection;
    ContextMenuHandler* contextMenuHandler {nullptr};
    contextMenuHandler = new ContextMenuHandler(this);

    QMainWindow::connect(contextMenuHandler, &ContextMenuHandler::newGame, m_OpenGLWidget, &OpenGLWidget::onNewGame);
    csvConnection = QMainWindow::connect(contextMenuHandler, &ContextMenuHandler::exportCSV, [=](){
        std::vector<TMove> stack = m_OpenGLWidget->getMoveStack();
        if(!HistoryLogger::exportAsCSV(stack)) qWarning() << tr("Błąd podczas zapisu CSV");
    });

    jsonConnection = QMainWindow::connect(contextMenuHandler, &ContextMenuHandler::exportJSON, [=](){
        std::vector<TMove> stack = m_OpenGLWidget->getMoveStack();
        if(!HistoryLogger::exportAsJSON(stack)) qWarning() << tr("Błąd podczas zapisu CSV");
    });

    contextMenuHandler->move(QCursor::pos());
    contextMenuHandler->exec();

    QMainWindow::disconnect(contextMenuHandler, &ContextMenuHandler::newGame, m_OpenGLWidget, &OpenGLWidget::onNewGame);
    QMainWindow::disconnect(csvConnection);
    QMainWindow::disconnect(jsonConnection);

    delete contextMenuHandler;
}

