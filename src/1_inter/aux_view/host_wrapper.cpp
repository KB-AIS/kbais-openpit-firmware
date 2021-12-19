#include "host_wrapper.h"

using namespace KbAis::Cfw::Sensors::Gps;

constexpr int SCREEN_W { 1024 }, SCREEN_H { 640 };

HostWrapper::HostWrapper(const BaseGpsDeviceController& gps_controller) :
    navigationStack(new QStackedWidget),
    mainPresenter(new main_presenter(navigationStack)),
    diagPresenter(new diag_presenter(navigationStack)) {
    navigationStack->setGeometry(0, 0, SCREEN_W, SCREEN_H);
    navigationStack->setStyleSheet("QWidget { background-color: black; color: white }");
    navigationStack->insertWidget(static_cast<int>(NavStackIdx::main), mainPresenter);
    navigationStack->insertWidget(static_cast<int>(NavStackIdx::diag), diagPresenter);
    navigationStack->setCurrentIndex(static_cast<int>(NavStackIdx::main));
    navigationStack->show();

    connect(
        &gps_controller, &BaseGpsDeviceController::notifyGpsDataUpdated,
        mainPresenter, &main_presenter::handleGpsDataUpdated
    );
}

HostWrapper::~HostWrapper() {}

void HostWrapper::navigateToMain() {
    navigationStack->setCurrentIndex(static_cast<int>(NavStackIdx::main));
}

void HostWrapper::navigateToDiag() {
    navigationStack->setCurrentIndex(static_cast<int>(NavStackIdx::diag));
}
