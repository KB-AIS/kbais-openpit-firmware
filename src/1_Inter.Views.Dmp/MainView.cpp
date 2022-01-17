#include "MainView.h"

// std
#include <chrono>
// qt
#include <QDateTime>
#include <QLabel>

#include "ui_MainView.h"

using namespace std::chrono_literals;

const QString TIME_EVEN_FMT { "hh:mm" }, TIME_FMT { "hh mm" };

constexpr auto TIMER_UPDATE_TIME_INTERVAL { 1s };

MainView::MainView(const IRxGpsSensorPublisher& gpsSensorPublisher)
    : QWidget()
    , ui { new Ui::MainView }
    , tUpdateDisplayTime { new QTimer(this) }
{
    ui->setupUi(this);

    subs = rxcpp::composite_subscription();

    gpsSensorPublisher.getObservable()
        .subscribe(subs, [&](const GpsMessage& gpsMessage) {
            const auto text = QString("Time %1 Coords %2 %3")
                .arg(gpsMessage.datetime.toString("d M yyyy hh:mm:ss"))
                .arg(gpsMessage.latitude)
                .arg(gpsMessage.longitude);

            ui->lbl_diag->setText(text);
        });

    // Setup on screen timer update with 1 second interval.    
    tUpdateDisplayTime->setInterval(TIMER_UPDATE_TIME_INTERVAL);

    connect(
        tUpdateDisplayTime, &QTimer::timeout,
        this, [&] {
            const auto now { QDateTime::currentDateTime() };

            const auto delimeter = now.time().second() % 2;

            const auto time = delimeter ? now.toString(TIME_EVEN_FMT) : now.toString(TIME_FMT);

            ui->lbl_timer->setText(time);
        });

    tUpdateDisplayTime->start();

    connect(
        ui->btn_nav_to_stop, &QPushButton::released,
        this, &MainView::notifyTestUserEvent);
}

MainView::~MainView() {
    delete ui;
}
