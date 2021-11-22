#include "main_presenter.h"
#include "ui_main_view.h"

// std
#include <chrono>
// qt
#include <QDateTime>
#include <QLabel>
#include <QStringBuilder>
// plog
#include <plog/Log.h>

using Sensors::Gps::GpsUpdate;

const QString TIME_EVEN_FMT { "hh:mm" };

const QString TIME_FMT { "hh mm" };

main_presenter::main_presenter(QWidget *parent) : QWidget(parent),
    ui(new Ui::main_presenter) {
    ui->setupUi(this);

    // Setup on screen timer update with 1 second interval.
    m_timer_update_time = new QTimer(this);

    connect(
        m_timer_update_time, &QTimer::timeout,
        this, [&] {
            const auto now { QDateTime::currentDateTime() };

            const auto print_delimeter = now.time().second() % 2;

            const auto time = print_delimeter
                ? now.toString(TIME_EVEN_FMT) : now.toString(TIME_FMT);

            ui->lbl_timer->setText(time);
        }
    );

    const auto timer_update_interval { std::chrono::seconds(1) };

    m_timer_update_time->start(timer_update_interval);

    // TODO: Setup the rest of presenter
}

main_presenter::~main_presenter() {
    delete ui;
}

void main_presenter::update_gps_data_slot(const GpsUpdate& update) {
    const auto text = QString("Time %1 Coords %2 %3")
            .arg(update.datetime.toString("d M yyyy hh:mm:ss"))
            .arg(update.latitude)
            .arg(update.longitude);

    ui->lbl_diag->setText(text);
}
