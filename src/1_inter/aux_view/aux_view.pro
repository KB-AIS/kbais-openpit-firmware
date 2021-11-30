PROJDIR = $${PWD}/../..

TARGET = aux-view

QT += core gui widgets

TEMPLATE = lib

CONFIG += c++17

INCLUDEPATH += \
    # carrier-firmware
    $${PROJDIR}/1_infra/gps_sensor_sp \
    $${PROJDIR}/2_app/gps_sensor

HEADERS += \
    diag_presenter.h \
    host_wrapper.h \
    main_presenter.h

SOURCES += \
    diag_presenter.cpp \
    host_wrapper.cpp \
    main_presenter.cpp

FORMS += \
    diag_view.ui \
    main_view.ui

LIBS += -L$${PROJDIR}/binaries \
    -lgps-sensor \
    -lgps-sensor-sp

message(1_inter/aux_view proj dir: $${PWD})

DESTDIR     = $${PROJDIR}/binaries
OBJECTS_DIR = $${PWD}/build/.obj
MOC_DIR     = $${PWD}/build/.moc
RCC_DIR     = $${PWD}/build/.qrc
UI_DIR      = $${PWD}/build/.ui

message(1_inter/aux_view  dest dir: $${DESTDIR})

target.path += /usr/lib
INSTALLS += target
