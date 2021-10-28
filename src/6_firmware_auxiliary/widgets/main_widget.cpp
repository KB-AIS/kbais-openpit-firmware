#include <QDateTime>
#include <QDebug>
#include <QLabel>
#include <QMovie>
#include <QPushButton>
#include <QTimer>

#include "main_widget.h"
#include "ui_host_widget.h"

host_widget::host_widget(QWidget *parent) :
    QWidget(parent), ui(new Ui::host_widget) {
    ui->setupUi(this);

    QMovie *pepga = new QMovie(":/memes/pepega.gif");
    ui->label->setMovie(pepga);
    pepga->start();

    m_timer_update_time = new QTimer();
    m_timer_update_time->setInterval(1000);
    connect(
        m_timer_update_time, &QTimer::timeout,
        [c_ui = ui] {
            QDateTime now(QDateTime::currentDateTime());

            int needDrawPoints = now.time().second() % 2;

            c_ui->lbl_time->setText(needDrawPoints ? now.toString("hh:mm") : now.toString("hh mm"));
        }
    );
    m_timer_update_time->start();
}

host_widget::~host_widget() {
    delete ui;

    delete m_timer_update_time;
}
