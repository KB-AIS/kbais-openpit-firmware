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
    opf/modules/sensors/crd/crd_formatter_tkcardreader_modbus.h \
    opf/modules/sensors/crd/crd_sensor_integrator_tkcardreader.h \
    opf/modules/sensors/crd/crd_sensor_publisher.h \
    opf/modules/sensors/flv/flv_calibration_publisher.h \
    opf/modules/sensors/flv/flv_message_publisher.h \
    opf/modules/sensors/gps/gpio_utils.h \
    opf/modules/sensors/gps/gps_sensor_publisher.h \
    opf/modules/sensors/gps/nmea/NmeaParser.h \
    opf/modules/sensors/gps/nmea/NmeaSentence.h \
    opf/modules/sensors/lcs/lcs_sensor_integrator_wtruck.h \
    opf/modules/sensors/lcs/lcs_sensor_publisher.h \
    opf/modules/sensors/lls/lls_formatter_omnicomm.h \
    opf/modules/sensors/lls/lls_sensor_integrator_omnicomm.h \
    opf/modules/sensors/lls/lls_sensor_publisher.h \
    opf/modules/sensors/module_factory_sensors.h \
    opf/modules/sensors/runner_module_sensors.h

SOURCES += \
    opf/modules/sensors/crd/crd_formatter_tkcardreader_modbus.cpp \
    opf/modules/sensors/crd/crd_sensor_publisher.cpp \
    opf/modules/sensors/flv/flv_calibration_publisher.cpp \
    opf/modules/sensors/gps/gpio_utils.cpp \
    opf/modules/sensors/gps/gps_sensor_publisher.cpp \
    opf/modules/sensors/gps/nmea/NmeaParser.cpp \
    opf/modules/sensors/gps/nmea/NmeaSentence.cpp \
    opf/modules/sensors/lcs/lcs_sensor_publisher.cpp \
    opf/modules/sensors/lls/lls_formatter_omnicomm.cpp \
    opf/modules/sensors/lls/lls_sensor_publisher.cpp \
    opf/modules/sensors/runner_module_sensors.cpp

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
