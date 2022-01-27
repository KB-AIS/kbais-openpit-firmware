PRJDIR = $$PWD/..

TARGET = OpenPitDmp

QT += core gui widgets

TEMPLATE = app

CONFIG += \
    c++11 c++14 c++17 \
    depend_includepath \
    conan_basic_setup \

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.App.Dmp.Presentation \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Messaging \
    $$PRJDIR/KbAis.OpenPit.Firmware.Modules.Legacy.Agtp \
    $$PRJDIR/KbAis.OpenPit.Firmware.Modules.Sensors.Gps \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.Extensions \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.BoostDi \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.JsonQt \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.RxQt \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    InjectorFactory.h \
    Messaging/DmpImmediateMessageMapper.h \
    Messaging/DmpRecurrentMessageMapper.h

SOURCES += \
    Messaging/DmpImmediateMessageMapper.cpp \
    Messaging/DmpRecurrentMessageMapper.cpp \
    main.cpp \

LIBS += -L$$PRJDIR/binaries \
    -lOpenPitAppDmpPresentation \
    -lOpenPitCoreConfiguration \
    -lOpenPitCoreMessaging \
    -lOpenPitModulesLegacyAgtp \
    -lOpenPitModulesSensorsGps \
    -lOpenPitUtilsExtensions \
    -lOpenPitUtilsTrdPartyBoostDi \
    -lOpenPitUtilsTrdPartyJsonQt \
    -lOpenPitUtilsTrdPartyRxQt \

DESTDIR     = $$PWD/../binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/sbin
INSTALLS += target
