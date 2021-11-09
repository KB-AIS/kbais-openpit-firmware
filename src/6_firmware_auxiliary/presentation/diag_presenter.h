#ifndef DIAG_PRESENTER_H
#define DIAG_PRESENTER_H

// qt
#include <QWidget>

namespace Ui {

class diag_presenter;

}

class diag_presenter : public QWidget {
    Q_OBJECT

public:
    diag_presenter(QWidget *parent = nullptr);

    ~diag_presenter();

private:
    Ui::diag_presenter *ui;
};

#endif // DIAG_PRESENTER_H
