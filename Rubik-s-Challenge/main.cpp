#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QFile file(":/style/style.qss");

    if(file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        QString style = stream.readAll();

        w.setStyleSheet(style);
    }

    a.setWindowIcon(QIcon(":/assets/icon/icon.png"));
    w.show();
    return a.exec();
}
