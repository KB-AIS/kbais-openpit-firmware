#ifndef MAIN_VIEW_DIALOG_NAV_H
#define MAIN_VIEW_DIALOG_NAV_H

#include <QtWidgets/qdialog.h>

namespace Ui { class main_view_dialog_nav; }

class main_view_dialog_nav : public QDialog {
    Q_OBJECT

    Ui::main_view_dialog_nav* ui;

public:
    explicit main_view_dialog_nav(QWidget *parent = nullptr);

    ~main_view_dialog_nav();

};

#endif // MAIN_VIEW_DIALOG_NAV_H
