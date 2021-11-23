#include "host_wrapper.h"


#include <commands/setup_time_handler.h>
// plog
#include <plog/Log.h>

using namespace Sensors::Gps;

constexpr int SCREEN_W { 1024 }, SCREEN_H { 640 };

HostWrapper::HostWrapper(const GpsDeviceController& gps_controller) :
    m_nav_stack(new QStackedWidget),
    m_main_presenter(new main_presenter(m_nav_stack)),
    m_diag_presenter(new diag_presenter(m_nav_stack)),
    m_gps_controller(gps_controller) {

    m_nav_stack->setGeometry(0, 0, SCREEN_W, SCREEN_H);
    m_nav_stack->setStyleSheet("QWidget { background-color: black; color: white }");
    m_nav_stack->insertWidget(static_cast<int>(nav_stack_idx::main), m_main_presenter);
    m_nav_stack->insertWidget(static_cast<int>(nav_stack_idx::diag), m_diag_presenter);
    m_nav_stack->setCurrentIndex(static_cast<int>(nav_stack_idx::main));
    m_nav_stack->show();

    connect(
        &m_gps_controller, &GpsDeviceController::update_gps_data_signal,

        m_main_presenter, &main_presenter::update_gps_data_slot
    );
}

HostWrapper::~HostWrapper() {
    delete m_nav_stack;
}

void HostWrapper::navToMain() {
    m_nav_stack->setCurrentIndex(static_cast<int>(nav_stack_idx::main));
}

void HostWrapper::navToDiag() {
    m_nav_stack->setCurrentIndex(static_cast<int>(nav_stack_idx::diag));
}
