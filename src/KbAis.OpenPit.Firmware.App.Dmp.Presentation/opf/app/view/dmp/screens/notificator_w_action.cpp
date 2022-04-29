#include "notificator_w_action.h"
#include "ui_notificator_w_action.h"

// qt
#include <QtGui/qguiapplication.h>
#include <QtGui/qpainter.h>
#include <QtGui/qscreen.h>
// oss
#include <plog/Log.h>

using namespace std::literals::chrono_literals;

notificator_w_action::notificator_w_action(QWidget *parent)
    :   QWidget(parent)
    ,   ui_(new Ui::notificator_w_action)
{
    ui_->setupUi(this);

    anm_execute = new QPropertyAnimation(this, "geometry");
    anm_dismiss = new QPropertyAnimation(this, "geometry");
    // initially hide notification
    hide();

    ui_->lbl_dismiss_timout->setText("Сообщение закроется через 5 секунд");
    QObject::connect(&tmr_dismiss, &QTimer::timeout, this, [this]() {
        duration -= 1s;

        const auto left = QString("Сообщение закроется через %1 секунд").arg(duration.count());
        ui_->lbl_dismiss_timout->setText(left);

        if (duration <= 0s) {
            tmr_dismiss.stop();
        }
    });
}

notificator_w_action::~notificator_w_action() {
    delete ui_;
}

void notificator_w_action::execute(const ntf_prms&& ntf_prms) {
    prms_ = ntf_prms;

    ui_->lbl_primary_text->setText(prms_.primary_text);
    ui_->lbl_secondary_text->setText(prms_.secondary_text);

    // bring widget to the top of parent's stack
    raise();

    auto app_g = qApp->primaryScreen()->availableGeometry();
    auto own_g = geometry();

    anm_execute->setDuration(1000);
    anm_execute->setStartValue(QRect {
        app_g.right() - own_g.width() - 10
    ,   app_g.bottom()
    ,   own_g.width()
    ,   own_g.height()
    });
    anm_execute->setEndValue(QRect {
        app_g.right() - own_g.width() - 10
    ,   app_g.bottom() - own_g.height() - 16
    ,   own_g.width()
    ,   own_g.height()
    });
    anm_execute->setEasingCurve(QEasingCurve::InOutBack);

    anm_dismiss->setDuration(1000);
    anm_dismiss->setStartValue(QRect {
        app_g.right() - own_g.width() - 10
    ,   app_g.bottom() - own_g.height() - 16
    ,   own_g.width()
    ,   own_g.height()
    });
    anm_dismiss->setEndValue(QRect {
        app_g.right() + own_g.width() + 10
    ,   app_g.bottom() - own_g.height() - 14
    ,   own_g.width()
    ,   own_g.height()
    });
    anm_dismiss->setEasingCurve(QEasingCurve::InOutBack);

    show();

    duration = 5s;
    anm_execute->start();

    tmr_dismiss.start(1s);
    QTimer::singleShot(5s, this, [this]() {
        anm_dismiss->start();
    });
}

void notificator_w_action::mouseReleaseEvent(QMouseEvent*) {
    if (prms_.action_opt) {
        (*prms_.action_opt)();
    }
}

void notificator_w_action::paintEvent(QPaintEvent*) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
