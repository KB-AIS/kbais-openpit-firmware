#ifndef MAIN_WEIGHT_VIEW_H
#define MAIN_WEIGHT_VIEW_H

#include <QWidget>

class main_weight_view : public QWidget {

    QString svg_;

public:
    main_weight_view(QWidget* parent = 0);

    void set_svg(const QString& svg);

protected:
    void paintEvent(QPaintEvent *event) override;

};

#endif // MAIN_WEIGHT_VIEW_H
