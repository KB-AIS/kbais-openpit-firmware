PRJDIR = $$PWD/..

TARGET = OpenPitModulesSensorsSerials

TEMPLATE = lib

QT += core serialport \

CONFIG += staticlib depend_includepath conan_basic_setup \

QMAKE_CXXFLAGS += -std=gnu++17 -Wno-psabi -faligned-new \

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Configuration \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.Extensions \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.BoostDi \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.JsonQt \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.RxQt \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.Widgets \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    modules/sensors/serials/crd/crd_formatter_tkcardreader_modbus.h \
    modules/sensors/serials/crd/crd_sensor_integrator_tkcardreader.h \
    modules/sensors/serials/crd/crd_sensor_publisher.h \
    modules/sensors/serials/lcs/lcs_sensor_integrator_wtruck.h \
    modules/sensors/serials/lcs/lcs_sensor_publisher.h \
    modules/sensors/serials/lls/flv_calibration_publisher.h \
    modules/sensors/serials/lls/lls_formatter_omnicomm.h \
    modules/sensors/serials/lls/lls_sensor_integrator_omnicomm.h \
    modules/sensors/serials/lls/lls_sensor_publisher.h \
    modules/sensors/serials/module_factory_sensors_serials.h  \
    modules/sensors/serials/runner_module_sensors_serials.h

SOURCES += \
    modules/sensors/serials/crd/crd_formatter_tkcardreader_modbus.cpp \
    modules/sensors/serials/crd/crd_sensor_publisher.cpp \
    modules/sensors/serials/lcs/lcs_sensor_publisher.cpp \
    modules/sensors/serials/lls/flv_calibration_publisher.cpp \
    modules/sensors/serials/lls/lls_formatter_omnicomm.cpp \
    modules/sensors/serials/lls/lls_sensor_publisher.cpp \
    modules/sensors/serials/runner_module_sensors_serials.cpp

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
