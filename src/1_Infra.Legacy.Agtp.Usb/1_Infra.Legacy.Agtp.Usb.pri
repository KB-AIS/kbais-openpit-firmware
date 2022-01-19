CONFIG += 11 14 17

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/Handlers/FetchDeviceInfoCommandHandler.h \
    $$PWD/Handlers/FetchFullDeviceInfoCommandHandler.h \
    $$PWD/Handlers/FetchSupportModulesCommandHandler.h \
    $$PWD/Handlers/IAgtpCommandHandler.h \
    $$PWD/AgtpProtocolSerializer.h \
    $$PWD/CrcCalculators.h \
    $$PWD/IAgtpCommandsMediator.h \
    $$PWD/IAgtpCommandsReciever.h \
    $$PWD/UsbAgtpCommandsReciever.h \

SOURCES += \
    $$PWD/Handlers/FetchDeviceInfoCommandHandler.cpp \
    $$PWD/Handlers/FetchFullDeviceInfoCommandHandler.cpp \
    $$PWD/Handlers/FetchSupportModulesCommandHandler.cpp \
    $$PWD/AgtpProtocolSerializer.cpp \
    $$PWD/CrcCalculators.cpp \
    $$PWD/UsbAgtpCommandsReciever.cpp \
