#ifndef STEPTRACKER_H
#define STEPTRACKER_H

#include <QObject>
#include <QTableView>
#include <QList>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QHeaderView>

#include "directionsTypes.h"
#include "TMove.h"


class StepTracker : public QTableView
{
    Q_OBJECT
public:
    explicit StepTracker(QWidget *parent = nullptr);
    ~StepTracker();

private:
    QStandardItemModel* m_model;
    const int COLUMNS = 2;

    void initColumns();

public slots:

    void onAddItem(
        const int& id,
        const Direction& direction,
        const char& axis
    );

    void back();


};

#endif // STEPTRACKER_H
