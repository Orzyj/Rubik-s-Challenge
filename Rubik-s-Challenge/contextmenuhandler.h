#ifndef CONTEXTMENUHANDLER_H
#define CONTEXTMENUHANDLER_H

#include <QDialog>

namespace Ui {
class ContextMenuHandler;
}

class ContextMenuHandler : public QDialog
{
    Q_OBJECT

public:
    explicit ContextMenuHandler(QWidget *parent = nullptr);
    ~ContextMenuHandler();

private:
    Ui::ContextMenuHandler *ui;
};

#endif // CONTEXTMENUHANDLER_H
