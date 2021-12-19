#include "main_presenter.h"

// std
#include <chrono>
// qt
#include <QDateTime>
#include <QLabel>
#include <QStringBuilder>

#include "ui_main_view.h"

using KbAis::Cfw::Sensors::Gps::GpsUpdate;

const QString TIME_EVEN_FMT { "hh:mm" };

const QString TIME_FMT { "hh mm" };

main_presenter::main_presenter(QWidget *parent) : QWidget(parent),
    ui(new Ui::main_presenter) {
    ui->setupUi(this);

    // Setup on screen timer update with 1 second interval.
    timerUpdateTime = new QTimer(this);

    connect(
        timerUpdateTime, &QTimer::timeout,
        this, [&] {
            const auto now { QDateTime::currentDateTime() };

            const auto print_delimeter = now.time().second() % 2;

            const auto time = print_delimeter
                ? now.toString(TIME_EVEN_FMT) : now.toString(TIME_FMT);

            ui->lbl_timer->setText(time);
        }
    );

    const auto timer_update_interval { std::chrono::seconds(1) };

    timerUpdateTime->start(timer_update_interval);

    connect(
        ui->btn_nav_to_stop, &QPushButton::released,
        this, [&] { emit notifyTestUserEvent(); }
    );
}

main_presenter::~main_presenter() {
    delete ui;
}

void main_presenter::handleGpsDataUpdated(const GpsUpdate& update) {
    const auto text = QString("Time %1 Coords %2 %3")
            .arg(update.datetime.toString("d M yyyy hh:mm:ss"))
            .arg(update.latitude)
            .arg(update.longitude);

    ui->lbl_diag->setText(text);
}
