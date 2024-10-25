#ifndef STEPTRACKER_H
#define STEPTRACKER_H

#include <QObject>
#include <QTableView>
#include <QList>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QHeaderView>

class StepTracker : public QTableView
{
    Q_OBJECT
public:
    explicit StepTracker(QWidget *parent = nullptr);
    ~StepTracker();

private:
    QStandardItemModel* m_model;
    const int COLUMNS = 3;

    void initColumns();

public slots:
    void onAddItem(QString itemValue);
};

#endif // STEPTRACKER_H
