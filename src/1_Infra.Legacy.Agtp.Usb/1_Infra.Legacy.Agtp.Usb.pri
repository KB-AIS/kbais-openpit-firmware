CONFIG += 11 14 17

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/AgtpCommandsMediator.h \
    $$PWD/AgtpCommandsParser.h \
    $$PWD/AgtpUsbCommandsReciever.h \
    $$PWD/DeviceStateCollector.h \
    $$PWD/Handlers/FetchDeviceInfoCmdHandler.h \
    $$PWD/Handlers/FetchDeviceModulesCmdHandler.h \
    $$PWD/Handlers/FetchDeviceSpecCmdHandler.h \
    $$PWD/Handlers/FetchDeviceStateCmdHandler.h \
    $$PWD/Handlers/IAgtpCommandHandler.h \
    $$PWD/IAgtpCommandsMediator.h \
    $$PWD/IAgtpCommandsReciever.h \
    $$PWD/ModuleAgtpService.h \

SOURCES += \
    $$PWD/AgtpCommandsMediator.cpp \
    $$PWD/AgtpCommandsParser.cpp \
    $$PWD/AgtpUsbCommandsReciever.cpp \
    $$PWD/DeviceStateCollector.cpp \
    $$PWD/Handlers/FetchDeviceInfoCmdHandler.cpp \
    $$PWD/Handlers/FetchDeviceModulesCmdHandler.cpp \
    $$PWD/Handlers/FetchDeviceSpecCmdHandler.cpp \
    $$PWD/Handlers/FetchDeviceStateCmdHandler.cpp \
