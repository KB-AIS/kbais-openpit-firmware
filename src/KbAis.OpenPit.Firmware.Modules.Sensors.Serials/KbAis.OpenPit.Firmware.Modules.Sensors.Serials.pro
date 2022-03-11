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
    Modules/Sensors/Serials/ModuleBootstraperSerialDevices.h \
    Modules/Sensors/Serials/ModuleFactorySerialDevices.h \
    Modules/Sensors/Serials/OmnicommLlsProtocolFomratter.h \
    Modules/Sensors/Serials/RxFuelMessagePublisher.h \
    Modules/Sensors/Serials/SerialRxLcsSensorPublisher.h \
    Modules/Sensors/Serials/SerialRxLlsSensorPublisher.h

SOURCES += \
    Modules/Sensors/Serials/ModuleBootstraperSerialDevices.cpp \
    Modules/Sensors/Serials/OmnicommLlsProtocolFomratter.cpp \
    Modules/Sensors/Serials/RxFuelMessagePublisher.cpp \
    Modules/Sensors/Serials/SerialRxLcsSensorPublisher.cpp \
    Modules/Sensors/Serials/SerialRxLlsSensorPublisher.cpp

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
