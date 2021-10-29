#include "main_form.h"
#include "ui_main_form.h"

main_form::main_form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::main_form)
{
    ui->setupUi(this);

    m_timer_update_time = new QTimer();

    connect(
        m_timer_update_time, &QTimer::timeout,
        [c_ui = ui] {
            auto now(QDateTime::currentDateTime());

            auto needPrintDelimiter = now.time().second() % 2;

            const QString TimeEventFmt = "hh:mm";

            const QString TimeFmt = "hh mm";

            auto time =
                needPrintDelimiter ? now.toString(TimeEventFmt) : now.toString(TimeFmt);

            c_ui->lbl_time->setText(time);
        }
    );

    m_timer_update_time->setInterval(1000);
    m_timer_update_time->start();
}

main_form::~main_form()
{
    delete ui;

    delete m_timer_update_time;
}
