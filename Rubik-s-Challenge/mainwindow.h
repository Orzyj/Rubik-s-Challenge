#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QLabel>
#include <QEvent>
#include <QHBoxLayout>

#include "openglwidget.h"
#include "scrollhandle.h"
#include "keyboardhandle.h"
#include "mousehandle.h"
#include "steptracker.h"
#include "contextmenuhandler.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void scrolledUp(OpenGLWidget&  widget);
    void scrolledDown(OpenGLWidget&  widget);
    void keyPressed(const int &key, const Qt::KeyboardModifiers &modifier, OpenGLWidget &widget);
    void keyRelesed(const int &key);
    void mousePressed(const QRect& geometry, const QPoint& mousePosition, OpenGLWidget& widget);
    void mouseRelesed();
    void mouseMoved(const QPoint& newPosition, OpenGLWidget& widget);
private:
    Ui::MainWindow *ui;
    OpenGLWidget *m_OpenGLWidget {nullptr};
    ScrollHandle* m_scrollHandle {nullptr};
    KeyboardHandle* m_keyboardHandle {nullptr};
    MouseHandle* m_mouseHandle {nullptr};
    StepTracker* m_stepTrackerListWidget {nullptr};

    void baseConfiguration();
    void onRightButtonMouse(const QPoint& mousePosition);

private slots:
    void onLabelXTextChanged(const float& value);
    void onLabelYTextChanged(const float& value);
    void onCheckBoxAxisStateChanged(const bool& state);
    void on_btnBackSteps_clicked();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

};
#endif // MAINWINDOW_H
