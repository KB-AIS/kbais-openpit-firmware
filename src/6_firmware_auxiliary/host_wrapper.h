#ifndef HOST_WRAPPER_H
#define HOST_WRAPPER_H


#include "gps_device_controller.h"

#include "presentation/diag_presenter.h"
#include "presentation/main_presenter.h"

// qt
#include <QObject>
#include <QStackedWidget>

enum class nav_stack_idx { main = 0, diag = 1, };

class HostWrapper : public QObject {
    Q_OBJECT

public:
    HostWrapper(const Sensors::Gps::GpsDeviceController&);

    ~HostWrapper();

    void navToMain();

    void navToDiag();

private:
    QStackedWidget* m_nav_stack;

    main_presenter* m_main_presenter;

    diag_presenter* m_diag_presenter;

    const Sensors::Gps::GpsDeviceController& m_gps_controller;
};

#endif // HOST_WRAPPER_H
