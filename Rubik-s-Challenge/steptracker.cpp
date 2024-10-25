#include "steptracker.h"

StepTracker::StepTracker(QWidget *parent)
    : QTableView{parent}
{
    this->m_model = new QStandardItemModel(0, COLUMNS);
    initColumns();

}

StepTracker::~StepTracker()
{
    if(m_model != nullptr) {
        delete m_model;
    }
}

void StepTracker::initColumns()
{
    std::vector<QString> columns = {"ID", "Ruch" , "Oś"};
    const float WIDTH = this->width();

    for(int i = 0; i < columns.size(); i++) {
        m_model->setHeaderData(i, Qt::Horizontal, columns[i]);
        this->setColumnWidth(i, static_cast<int>(WIDTH / COLUMNS));
    }

    this->setModel(m_model);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void StepTracker::onAddItem(QString itemValue)
{
    try {
        QList<QStandardItem*> row;
        const int ROW_COUNT = m_model->rowCount();

        row << new QStandardItem(QString::number(ROW_COUNT));
        row << new QStandardItem(itemValue);

        m_model->appendRow(row);
    }
    catch (const std::bad_alloc& e) {
        qWarning() << "Błąd alokacji pamięci:" << e.what();
    }
}
