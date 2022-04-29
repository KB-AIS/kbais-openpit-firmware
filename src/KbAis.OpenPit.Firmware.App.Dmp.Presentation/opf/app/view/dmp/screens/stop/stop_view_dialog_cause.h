#ifndef STOP_VIEW_DIALOG_CAUSE_H
#define STOP_VIEW_DIALOG_CAUSE_H

#include <QDialog>

namespace Ui { class stop_view_dialog_reason; }

class stop_view_dialog_cause : public QDialog
{
    Q_OBJECT

public:
    explicit stop_view_dialog_cause(QWidget *parent = nullptr);
    ~stop_view_dialog_cause();

private:
    Ui::stop_view_dialog_reason *ui;
};

#endif // STOP_VIEW_DIALOG_CAUSE_H
