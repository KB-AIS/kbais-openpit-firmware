#include "main_presenter.h"

// std
#include <chrono>
// qt
#include <QDateTime>
#include <QLabel>

#include "ui_main_view.h"

using namespace std::chrono_literals;

const QString TIME_EVEN_FMT { "hh:mm" }, TIME_FMT { "hh mm" };

constexpr auto TIMER_UPDATE_TIME_INTERVAL { 1s };

main_presenter::main_presenter(QWidget *parent) : QWidget(parent),
    ui(new Ui::main_presenter), timerUpdateTime(new QTimer(this)) {
    ui->setupUi(this);

    // Setup on screen timer update with 1 second interval.    
    timerUpdateTime->setInterval(TIMER_UPDATE_TIME_INTERVAL);

    connect(
        timerUpdateTime, &QTimer::timeout,
        this, [&] {
            const auto now { QDateTime::currentDateTime() };

            const auto print_delimeter = now.time().second() % 2;

            const auto time = print_delimeter
                ? now.toString(TIME_EVEN_FMT) : now.toString(TIME_FMT);

            ui->lbl_timer->setText(time);
        });

    timerUpdateTime->start();

    connect(
        ui->btn_nav_to_stop, &QPushButton::released,
        this, &main_presenter::notifyTestUserEvent);
}

main_presenter::~main_presenter() {
    delete ui;
}

void main_presenter::handleGpsDataUpdated(const GpsUpdateDto& gpsUpdate) {
    const auto text = QString("Time %1 Coords %2 %3")
        .arg(gpsUpdate.datetime.toString("d M yyyy hh:mm:ss"))
        .arg(gpsUpdate.latitude)
        .arg(gpsUpdate.longitude);

    ui->lbl_diag->setText(text);
}
