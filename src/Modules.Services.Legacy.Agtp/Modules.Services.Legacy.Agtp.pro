PRJDIR = $${PWD}/..

QT += core serialport

CONFIG += 11 14 17 conan_basic_setup

TEMPLATE = lib

include($${PRJDIR}/../conanbuildinfo.pri)
include($${PRJDIR}/TrdParty/TrdParty.pri)
include($${PRJDIR}/Utils/Utils.pri)
include($${PRJDIR}/Modules.Sensors.Gps/Modules.Sensors.Gps.pri)
include($${PRJDIR}/Core.Configuration/Core.Configuration.pri)

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
