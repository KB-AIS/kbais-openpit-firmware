#ifndef NOTIFICATOR_W_ACTION_H
#define NOTIFICATOR_W_ACTION_H

// std
#include <chrono>
#include <functional>
// qt
#include <QtCore/qpropertyanimation.h>
#include <QtCore/qtimer.h>
#include <QtWidgets/qlabel.h>

#include "opf/app/view/dmp/nav/ntf_controller.h"

namespace Ui { class notificator_w_action; }

class notificator_w_action : public QWidget {
    Q_OBJECT

    Ui::notificator_w_action* ui_;

    ntf_prms prms_;

    std::chrono::seconds duration { 5ll };

    QPropertyAnimation* anm_execute;

    QPropertyAnimation* anm_dismiss;

    QTimer tmr_dismiss;

public:
    explicit notificator_w_action(QWidget* parent = nullptr);

    ~notificator_w_action();

    void execute(const ntf_prms&& ntf_prms);

    void dismiss();

protected:
    void mouseReleaseEvent(QMouseEvent* event);

    void paintEvent(QPaintEvent* event);

};

#endif // NOTIFICATOR_W_ACTION_H
