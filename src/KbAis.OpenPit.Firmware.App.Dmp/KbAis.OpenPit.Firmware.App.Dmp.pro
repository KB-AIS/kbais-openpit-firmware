PRJDIR = $$PWD/..

TARGET = OpenPitDmp

TEMPLATE = app

QT += core gui network serialport sql widgets \

CONFIG += depend_includepath conan_basic_setup \

QMAKE_CXXFLAGS += -std=gnu++17 -Wno-psabi -faligned-new \

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.App.Dmp.Presentation \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Configuration \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.EventBus \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Messaging \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Networking \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Persisting \
    $$PRJDIR/KbAis.OpenPit.Firmware.Modules.Legacy.Agtp \
    $$PRJDIR/KbAis.OpenPit.Firmware.Modules.Sensors.Gps \
    $$PRJDIR/KbAis.OpenPit.Firmware.Modules.Sensors.Serials \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.Extensions \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.BoostDi \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.JsonQt \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.RxQt \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.Widgets \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    CompositionRootModule.h \
    ConfiguratorCommandLine.h \
    FormatterModbusCardReader.h \
    Messaging/DmpImmediateMessageMapper.h \
    Messaging/DmpRecurrentMessageMapper.h \
    RxStateWatcherFueling.h \
    System/SetupDateTimeService.h \
    System/SetupEthernetService.h \
    SerialRxRcrSensorPublisher.h

SOURCES += \
    ConfiguratorCommandLine.cpp \
    FormatterModbusCardReader.cpp \
    Messaging/DmpImmediateMessageMapper.cpp \
    Messaging/DmpRecurrentMessageMapper.cpp \
    RxStateWatcherFueling.cpp \
    System/SetupDateTimeService.cpp \
    System/SetupEthernetService.cpp \
    main.cpp \
    SerialRxRcrSensorPublisher.cpp

LIBS += -L$$PRJDIR/binaries \
    -lOpenPitAppDmpPresentation \
    -lOpenPitCoreConfiguration \
    -lOpenPitCoreEventBus \
    -lOpenPitCoreMessaging \
    -lOpenPitCoreNetworking \
    -lOpenPitCorePersisting \
    -lOpenPitModulesLegacyAgtp \
    -lOpenPitModulesSensorsGps \
    -lOpenPitModulesSensorsSerials \
    -lOpenPitUtilsExtensions \
    -lOpenPitUtilsTrdPartyBoostDi \
    -lOpenPitUtilsTrdPartyJsonQt \
    -lOpenPitUtilsTrdPartyRxQt \
    -lOpenPitAppDmpUtilsWidgets \

DESTDIR     = $$PWD/../binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/sbin
INSTALLS += target
