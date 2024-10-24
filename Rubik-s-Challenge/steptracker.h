#ifndef STEPTRACKER_H
#define STEPTRACKER_H

#include <QObject>

class StepTracker : public QObject
{
    Q_OBJECT
public:
    explicit StepTracker(QObject *parent = nullptr);

signals:
};

#endif // STEPTRACKER_H
