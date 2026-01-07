#include "contextmenuhandler.h"
#include "ui_contextmenuhandler.h"

ContextMenuHandler::ContextMenuHandler(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ContextMenuHandler)
{
    ui->setupUi(this);

    QWidget::connect(ui->btnStartAgain, &QPushButton::clicked, this, &ContextMenuHandler::onNewGameButtonClicked);
    QWidget::connect(ui->btnExportCSV, &QPushButton::clicked, [this](){ emit exportCSV();});
    QWidget::connect(ui->btnExportJSON, &QPushButton::clicked, [this](){ emit exportJSON(); });
}

ContextMenuHandler::~ContextMenuHandler()
{
    delete ui;
}

void ContextMenuHandler::onNewGameButtonClicked()
{
    emit newGame();
}
