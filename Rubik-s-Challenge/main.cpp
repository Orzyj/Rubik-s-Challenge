#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>

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


    w.show();
    return a.exec();
}
