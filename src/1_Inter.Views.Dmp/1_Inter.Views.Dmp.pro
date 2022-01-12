PRJDIR = $${PWD}/..

TARGET = cfw_inter_views_dmp

QT += core gui widgets

TEMPLATE = lib

CONFIG += c++11 c++14 c++17 conan_basic_setup

INCLUDEPATH += \
    # cfw
    $${PRJDIR}/1_Infra.Sensors.Gps \

include($${PRJDIR}/../conanbuildinfo.pri)

HEADERS += \
    diag_presenter.h \
    host_wrapper.h \
    main_presenter.h \

SOURCES += \
    diag_presenter.cpp \
    host_wrapper.cpp \
    main_presenter.cpp \

FORMS += \
    diag_view.ui \
    main_view.ui \

LIBS += -L$${PRJDIR}/binaries \
    -lcfw_infra_sensors_gps \

DESTDIR     = $${PRJDIR}/binaries
OBJECTS_DIR = $${PWD}/build/.obj
MOC_DIR     = $${PWD}/build/.moc
RCC_DIR     = $${PWD}/build/.qrc
UI_DIR      = $${PWD}/build/.ui

target.path += /usr/lib
INSTALLS += target
