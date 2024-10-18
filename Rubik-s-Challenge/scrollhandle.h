#ifndef SCROLLHANDLE_H
#define SCROLLHANDLE_H

#include <QObject>

class ScrollHandle : public QObject
{
    Q_OBJECT
public:
    explicit ScrollHandle(QObject *parent = nullptr);

signals:
};

#endif // SCROLLHANDLE_H
