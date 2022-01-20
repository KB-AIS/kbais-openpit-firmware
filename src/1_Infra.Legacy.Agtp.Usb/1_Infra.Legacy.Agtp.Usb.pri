CONFIG += 11 14 17

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/AgtpCommandsMediator.h \
    $$PWD/AgtpCommandsParser.h \
    $$PWD/AgtpUsbCommandsReciever.h \
    $$PWD/Handlers/FetchDeviceInfoCommandHandler.h \
    $$PWD/Handlers/FetchFullDeviceInfoCommandHandler.h \
    $$PWD/Handlers/FetchSupportModulesCommandHandler.h \
    $$PWD/Handlers/IAgtpCommandHandler.h \
    $$PWD/IAgtpCommandsMediator.h \
    $$PWD/IAgtpCommandsReciever.h \
    $$PWD/ModuleAgtpService.h \

SOURCES += \
    $$PWD/AgtpCommandsMediator.cpp \
    $$PWD/AgtpCommandsParser.cpp \
    $$PWD/AgtpUsbCommandsReciever.cpp \
    $$PWD/Handlers/FetchDeviceInfoCommandHandler.cpp \
    $$PWD/Handlers/FetchFullDeviceInfoCommandHandler.cpp \
    $$PWD/Handlers/FetchSupportModulesCommandHandler.cpp \
