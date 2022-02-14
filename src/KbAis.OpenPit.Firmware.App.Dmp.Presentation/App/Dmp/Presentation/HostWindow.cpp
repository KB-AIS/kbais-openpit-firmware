#include "HostWindow.h"
#include "ui_HostWindow.h"

// oss
#include <range/v3/all.hpp>

using namespace std::chrono_literals;

constexpr qint32 MAIN_VIEW_IDX { 0 }, DIAG_VIEW_IDX { 1 }, STOP_VIEW_IDX { 2 }, CONF_VIEW_IDX { 3 };

const QString TIME_EVEN_FMT { "hh:mm" }, TIME_FMT { "hh mm" };

const auto STYLE_INDICATOR_VALID = QString(R"(
    color: #b8bb26;
    background-color: #00000000;
)");

const auto STYLE_INDICATOR_INVALID = QString(R"(
    color: #fb4934;
    background-color: #00000000;
)");

constexpr auto DISPLAYTIME_UPDATE_INTERVAL { 1s };

HostWindow::HostWindow(
    const IRxGpsSensorPublisher& gpsDiagPub
,   const IRxMessageSendersDiagPub& netDiagPub
,   MainView& mainView
,   DiagView& diagView
,   NavController& navController
)
:   QMainWindow()
,   ui { new Ui::HostWindow }
,   m_gpsDiagPub { gpsDiagPub }
,   m_netDiagPub { netDiagPub }
,   m_mainView { mainView }
,   m_diagView { diagView }
,   m_navController { navController }
{
    ui->setupUi(this);

    SetupScreenStack();
    SetupAppBar();
}

HostWindow::~HostWindow() {
    m_rxSubs.unsubscribe();
    delete ui;
}

void HostWindow::SetupScreenStack() {
    m_mainView.setParent(ui->sw_nav);
    ui->sw_nav->insertWidget(MAIN_VIEW_IDX, &m_mainView);

    m_diagView.setParent(ui->sw_nav);
    ui->sw_nav->insertWidget(DIAG_VIEW_IDX, &m_diagView);

    ui->sw_nav->setCurrentIndex(MAIN_VIEW_IDX);

    m_navController.GetObservableNavRequested()
        .subscribe(m_rxSubs, [&](int destId) {
            // TODO: Typesafe navigation
            ui->sw_nav->setCurrentIndex(destId);
        });
}

void HostWindow::SetupAppBar() {
    const auto coordinator = m_rxRunLoop.observe_on_run_loop();

    m_gpsDiagPub.GetObservable()
        .observe_on(coordinator)
        .subscribe(m_rxSubs, [&](const GpsMessage& x) {
            ui->lbl_indicatorGps
              ->setStyleSheet(x.isValid ? STYLE_INDICATOR_VALID : STYLE_INDICATOR_INVALID);
        });

    m_netDiagPub.GetObservableDiagInfo()
        .observe_on(coordinator)
        .subscribe(m_rxSubs, [&](const std::vector<MessageSenderDiagInfo>& x) {
            auto isConnected = ranges::contains(x, QString { "ConnectedState" }, &MessageSenderDiagInfo::stateText);

            ui->lbl_indicatorSrv
              ->setStyleSheet(isConnected ? STYLE_INDICATOR_VALID : STYLE_INDICATOR_INVALID);
        });

    rxqt::from_signal(&m_tmUpdateDisplayTime, &QTimer::timeout)
        .subscribe(m_rxSubs, [&](auto) { UpdateDisplayTime(); });

    m_tmUpdateDisplayTime.start(DISPLAYTIME_UPDATE_INTERVAL);
}

void HostWindow::UpdateDisplayTime() {
    const auto now { QDateTime::currentDateTime() };
    const auto delimeter = now.time().second() % 2;

    ui->lbl_time->setText(delimeter ? now.toString(TIME_EVEN_FMT) : now.toString(TIME_FMT));
}
