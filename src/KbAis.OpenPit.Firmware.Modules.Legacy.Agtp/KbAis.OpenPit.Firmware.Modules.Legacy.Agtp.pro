PRJDIR = $$PWD/..

TARGET = OpenPitModulesLegacyAgtp

TEMPLATE = lib

QT += core serialport

QMAKE_CXXFLAGS += -Wno-psabi

CONFIG += \
    c++11 c++14 c++17 \
    depend_includepath \
    conan_basic_setup \

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.Modules.Sensors.Gps \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Configuration \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.Extensions \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.RxQt \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    AgtpRequestMediator.h \
    Handlers/ConfigEthernetRequestHandler.h \
    Handlers/FetchDeviceInfoRequestHandler.h \
    Handlers/FetchDeviceModulesRequestHandler.h \
    Handlers/FetchDeviceSpecRequestHandler.h \
    Handlers/FetchDeviceStateRequsetHandler.h \
    Handlers/IAgtpRequestHandler.h \
    AgtpProtocolPareser.h \
    AgtpServiceModuleFactory.h \
    AgtpThreadWorker.h \
    AgtpUsbCommandsReciever.h \
    DeviceStateCollector.h \
    IAgtpRequestsMediator.h \
    IAgtpRequestsReciever.h

SOURCES += \
    AgtpRequestMediator.cpp \
    Handlers/ConfigEthernetRequestHandler.cpp \
    Handlers/FetchDeviceInfoRequestHandler.cpp \
    Handlers/FetchDeviceModulesRequestHandler.cpp \
    Handlers/FetchDeviceSpecRequestHandler.cpp \
    AgtpProtocolPareser.cpp \
    AgtpThreadWorker.cpp \
    AgtpUsbCommandsReciever.cpp \
    DeviceStateCollector.cpp \
    Handlers/FetchDeviceStateRequsetHandler.cpp

DESTDIR     = $$PWD/../binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
