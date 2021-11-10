#include "host_wrapper.h"

// plog
#include <plog/Log.h>

using namespace Sensors::Gps;

constexpr int SCREEN_W { 1024 }, SCREEN_H { 640 };

host_wrapper::host_wrapper() :
    m_nav_stack(new QStackedWidget),
    m_main_presenter(new main_presenter(m_nav_stack)),
    m_diag_presenter(new diag_presenter(m_nav_stack)),
    m_gps_contoller(this) {

    m_nav_stack->setGeometry(0, 0, SCREEN_W, SCREEN_H);
    m_nav_stack->setStyleSheet("QWidget { background-color: black; color: white }");
    m_nav_stack->insertWidget(static_cast<int>(nav_stack_idx::main), m_main_presenter);
    m_nav_stack->insertWidget(static_cast<int>(nav_stack_idx::diag), m_diag_presenter);
    m_nav_stack->setCurrentIndex(static_cast<int>(nav_stack_idx::main));

    m_nav_stack->show();

    connect(
        &m_gps_contoller, &gps_device_controller::update_gps_data_signal,

        m_main_presenter, &main_presenter::update_gps_data_slot
    );
}

host_wrapper::~host_wrapper() {
    delete m_nav_stack;
}

void host_wrapper::nav_to_main() {
    m_nav_stack->setCurrentIndex(static_cast<int>(nav_stack_idx::main));
}

void host_wrapper::nav_to_diag() {
    m_nav_stack->setCurrentIndex(static_cast<int>(nav_stack_idx::diag));
}
