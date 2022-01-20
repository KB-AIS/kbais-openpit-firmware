PRJDIR = $${PWD}/..

QT += core serialport

CONFIG += 11 14 17 conan_basic_setup

TEMPLATE = lib

include($${PRJDIR}/../conanbuildinfo.pri)
include($${PRJDIR}/TrdParty/TrdParty.pri)
include($${PRJDIR}/Utils/Utils.pri)

HEADERS += \
    AgtpCommandsMediator.h \
    AgtpCommandsParser.h \
    AgtpUsbCommandsReciever.h \
    Handlers/FetchDeviceInfoCommandHandler.h \
    Handlers/FetchFullDeviceInfoCommandHandler.h \
    Handlers/FetchSupportModulesCommandHandler.h \
    Handlers/IAgtpCommandHandler.h \
    IAgtpCommandsMediator.h \
    IAgtpCommandsReciever.h \
    ModuleAgtpService.h \

SOURCES += \
    AgtpCommandsMediator.cpp \
    AgtpCommandsParser.cpp \
    AgtpUsbCommandsReciever.cpp \
    Handlers/FetchDeviceInfoCommandHandler.cpp \
    Handlers/FetchFullDeviceInfoCommandHandler.cpp \
    Handlers/FetchSupportModulesCommandHandler.cpp \
