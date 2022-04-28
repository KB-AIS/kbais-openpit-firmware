#ifndef STOP_VIEW_DIALOG_REASON_H
#define STOP_VIEW_DIALOG_REASON_H

#include <QDialog>

namespace Ui { class stop_view_dialog_reason; }

class stop_view_dialog_reason : public QDialog
{
    Q_OBJECT

public:
    explicit stop_view_dialog_reason(QWidget *parent = nullptr);
    ~stop_view_dialog_reason();

private:
    Ui::stop_view_dialog_reason *ui;
};

#endif // STOP_VIEW_DIALOG_REASON_H
