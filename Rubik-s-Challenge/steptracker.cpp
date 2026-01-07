#include "steptracker.h"

StepTracker::StepTracker(QWidget *parent)
    : QTableView{parent}
{
    this->m_model = new QStandardItemModel(0, COLUMNS);
    this->setFocusPolicy(Qt::NoFocus);
    this->horizontalHeader()->setStretchLastSection(true);
    this->verticalHeader()->setHighlightSections(false);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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
    std::vector<QString> columns = {tr("Ruch") , tr("Oś")};
    const float WIDTH = this->width();

    for(int i = 0; i < columns.size(); i++) {
        m_model->setHeaderData(i, Qt::Horizontal, columns[i]);
        this->setColumnWidth(i, static_cast<int>(WIDTH / COLUMNS));
    }

    this->setModel(m_model);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void StepTracker::onAddItem(
    const int& id,
    const Direction& direction,
    const char& axis)
{
    try {
        QList<QStandardItem*> row;
        QString direction_text = QString::fromStdString(getMoveText(direction));

        row << new QStandardItem(direction_text);
        row << new QStandardItem(QString(axis));

        this->m_model->appendRow(row);
    }
    catch (const std::bad_alloc& e) {
        qWarning() << "Błąd alokacji pamięci:" << e.what();
    }
}

void StepTracker::back()
{
    if(m_model == nullptr) return;
    int rowCount = m_model->rowCount();

    if(rowCount > 0) {
        m_model->removeRow(rowCount - 1);
    }
}
