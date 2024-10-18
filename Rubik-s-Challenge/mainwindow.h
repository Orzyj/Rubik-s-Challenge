#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QLabel>
#include "openglwidget.h"

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

private:
    Ui::MainWindow *ui;
    OpenGLWidget *m_OpenGLWidget {nullptr};

    bool m_isInWindowFocus {false};
    QPoint m_basePoint;
    const int m_sensitivityThreshold {3};
    bool m_reversalAxisX;
    bool m_reversalAxisY;
    bool spacePressed {false};

    void baseConfiguration();

private slots:
    void onLabelXTextChanged(const float value);
    void onLabelYTextChanged(const float value);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

};
#endif // MAINWINDOW_H
