#ifndef HOST_WRAPPER_H
#define HOST_WRAPPER_H

#include <presentation/diag_presenter.h>
#include <presentation/main_presenter.h>

// sensors
#include <gps_device_controller.h>

// qt
#include <QObject>
#include <QStackedWidget>

enum class nav_stack_idx { main = 0, diag = 1, };

class host_wrapper : public QObject {
    Q_OBJECT

public:
    host_wrapper();

    ~host_wrapper();

    /*!
     * \brief navigate_to_main
     */
    void nav_to_main();

    /*!
     * \brief navigate_to_diag
     */
    void nav_to_diag();

private:
    QStackedWidget* m_nav_stack;
    main_presenter* m_main_presenter;
    diag_presenter* m_diag_presenter;
    Sensors::Gps::GpsDeviceController m_gps_controller;
};

#endif // HOST_WRAPPER_H
