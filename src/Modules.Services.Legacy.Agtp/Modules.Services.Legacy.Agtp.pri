CONFIG += 11 14 17

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/AgtpRequestMediator.h \
    $$PWD/Handlers/FetchDeviceInfoRequestHandler.h \
    $$PWD/Handlers/FetchDeviceModulesRequestHandler.h \
    $$PWD/Handlers/FetchDeviceSpecRequestHandler.h \
    $$PWD/Handlers/FetchDeviceStateRequsetHandler.h \
    $$PWD/Handlers/IAgtpRequestHandler.h \
    $$PWD/AgtpProtocolPareser.h \
    $$PWD/AgtpServiceModuleFactory.h \
    $$PWD/AgtpThreadWorker.h \
    $$PWD/AgtpUsbCommandsReciever.h \
    $$PWD/DeviceStateCollector.h \
    $$PWD/IAgtpRequestsMediator.h \
    $$PWD/IAgtpRequestsReciever.h

SOURCES += \
    $$PWD/AgtpRequestMediator.cpp \
    $$PWD/Handlers/FetchDeviceInfoRequestHandler.cpp \
    $$PWD/Handlers/FetchDeviceModulesRequestHandler.cpp \
    $$PWD/Handlers/FetchDeviceSpecRequestHandler.cpp \
    $$PWD/AgtpProtocolPareser.cpp \
    $$PWD/AgtpThreadWorker.cpp \
    $$PWD/AgtpUsbCommandsReciever.cpp \
    $$PWD/DeviceStateCollector.cpp \
    $$PWD/Handlers/FetchDeviceStateRequsetHandler.cpp

