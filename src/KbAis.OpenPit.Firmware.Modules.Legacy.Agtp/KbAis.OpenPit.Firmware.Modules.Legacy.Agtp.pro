PRJDIR = $$PWD/..

TARGET = OpenPitModulesLegacyAgtp

TEMPLATE = lib

QT += core serialport \

CONFIG += staticlib conan_basic_setup \

QMAKE_CXXFLAGS += -std=gnu++17 -Wno-psabi \

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.Modules.Sensors.Gps \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Configuration \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.Extensions \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.RxQt \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    AgtpRequestMediator.h \
    Handlers/ConfigEthernetRequestHandler.h \
    Handlers/ConfigNetworkingRequestHandler.h \
    Handlers/ConfigScaleRequestHandler.h \
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
    Handlers/Utils.h \
    IAgtpRequestsMediator.h \
    IAgtpRequestsReciever.h

SOURCES += \
    AgtpRequestMediator.cpp \
    Handlers/ConfigEthernetRequestHandler.cpp \
    Handlers/ConfigNetworkingRequestHandler.cpp \
    Handlers/ConfigScaleRequestHandler.cpp \
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
