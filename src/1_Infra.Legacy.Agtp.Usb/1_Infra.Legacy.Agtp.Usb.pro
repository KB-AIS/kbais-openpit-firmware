PRJDIR = $${PWD}/..

QT += core serialport

CONFIG += 11 c++14 17 conan_basic_setup

TEMPLATE = lib

include($${PRJDIR}/../conanbuildinfo.pri)
include($${PRJDIR}/TrdParty/TrdParty.pri)
include($${PRJDIR}/Utils/Utils.pri)
include($${PRJDIR}/1_Infra.Sensors.Gps/1_Infra.Sensors.Gps.pri)

HEADERS += \
    AgtpCommandsParser.h \
    AgtpRequestSender.h \
    AgtpUsbCommandsReciever.h \
    DeviceStateCollector.h \
    Handlers/FetchDeviceInfoCmdHandler.h \
    Handlers/FetchDeviceModulesCmdHandler.h \
    Handlers/FetchDeviceSpecCmdHandler.h \
    Handlers/FetchDeviceStateCmdHandler.h \
    Handlers/IAgtpRequestHandler.h \
    IAgtpCommandsMediator.h \
    IAgtpCommandsReciever.h \
    ModuleAgtpService.h \

SOURCES += \
    AgtpCommandsParser.cpp \
    AgtpRequestSender.cpp \
    AgtpUsbCommandsReciever.cpp \
    DeviceStateCollector.cpp \
    Handlers/FetchDeviceInfoCmdHandler.cpp \
    Handlers/FetchDeviceModulesCmdHandler.cpp \
    Handlers/FetchDeviceSpecCmdHandler.cpp \
    Handlers/FetchDeviceStateCmdHandler.cpp \
