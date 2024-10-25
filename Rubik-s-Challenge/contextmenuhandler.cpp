#include "contextmenuhandler.h"
#include "ui_contextmenuhandler.h"

ContextMenuHandler::ContextMenuHandler(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ContextMenuHandler)
{
    ui->setupUi(this);
}

ContextMenuHandler::~ContextMenuHandler()
{
    delete ui;
}
