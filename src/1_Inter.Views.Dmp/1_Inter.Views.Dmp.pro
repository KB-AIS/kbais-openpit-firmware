PRJDIR = $${PWD}/..

TARGET = cfw_inter_views_dmp

QT += core gui widgets

TEMPLATE = lib

CONFIG += 11 14 17 conan_basic_setup

INCLUDEPATH += \
    # cfw
    $${PRJDIR}/Modules.Sensors.Gps \
    $${PRJDIR}/TrdParty \

include($${PRJDIR}/../conanbuildinfo.pri)

HEADERS += \
    MainView.h \
    ViewWrapper.h

SOURCES += \
    MainView.cpp \
    ViewWrapper.cpp

FORMS += \
    MainView.ui

LIBS += -L$${PRJDIR}/binaries \
    -lcfw_infra_sensors_gps \
    -ltrdparty \

DESTDIR     = $${PRJDIR}/binaries
OBJECTS_DIR = $${PWD}/build/.obj
MOC_DIR     = $${PWD}/build/.moc
RCC_DIR     = $${PWD}/build/.qrc
UI_DIR      = $${PWD}/build/.ui

target.path += /usr/lib
INSTALLS += target
