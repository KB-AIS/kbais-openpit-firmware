#include "MainView.h"

// std
#include <chrono>
// qt
#include <QDateTime>
#include <QLabel>

#include <range/v3/all.hpp>

#include "ui_MainView.h"

using namespace std::chrono_literals;

const QString TIME_EVEN_FMT { "hh:mm" }, TIME_FMT { "hh mm" };

constexpr auto TIMER_UPDATE_TIME_INTERVAL { 1s };

MainView::MainView(
    const IRxGpsSensorPublisher& gpsSensorPublisher
,   const IRxMessageSendersDiagPub& message_sender_diag_pub
)   : QWidget()
    , ui { new Ui::MainView }
    , tUpdateDisplayTime { new QTimer(this) }
{
    ui->setupUi(this);

    m_subscriptions = rxcpp::composite_subscription();

    gpsSensorPublisher.getObservable()
        .subscribe(m_subscriptions, [&](const GpsMessage& gpsMessage) {
            const auto text = QString("Time %1 Coords %2 %3")
                .arg(gpsMessage.datetime.toString("d M yyyy hh:mm:ss"))
                .arg(gpsMessage.latitude)
                .arg(gpsMessage.longitude);

            ui->lbl_diag->setText(text);
        });

    message_sender_diag_pub.get_diag_observable()
        .subscribe(
            m_subscriptions
        ,   [&](const std::vector<MessageSenderDiagInfo>& x) {
                QString text;

                ranges::for_each(x, [&](MessageSenderDiagInfo y) {
                    const auto diag_info_text = QString("%1 \t state: %2 \t eror: %3\n")
                        .arg(y.message_sender_name)
                        .arg(y.state_text)
                        .arg(y.error_text_opt.value_or("No Error"));

                    text.append(diag_info_text);
                });

                ui->lbl_diag_net->setText(text);
            }
        );

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
