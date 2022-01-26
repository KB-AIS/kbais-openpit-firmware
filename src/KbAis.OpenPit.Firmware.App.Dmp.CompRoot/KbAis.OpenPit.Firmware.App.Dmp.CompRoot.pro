PRJDIR = $$PWD/..

TARGET = openpit-app-dmp

QT += core gui widgets

TEMPLATE = app

CONFIG += \
    c++11 c++14 c++17 \
    depend_includepath \
    conan_basic_setup \

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.App.Dmp.Presentation \
    $$PRJDIR/KbAis.OpenPit.Firmware.Modules.Legacy.Agtp \
    $$PRJDIR/KbAis.OpenPit.Firmware.Modules.Sensors.Gps \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.BoostDi \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    InjectorFactory.h

SOURCES += \
    main.cpp \

LIBS += -L$$PRJDIR/binaries \
    -lopenpit-app-dmp-presentation \
    -lOpenPitCoreConfiguration \
    -lOpenPitModulesLegacyAgtp \
    -lopenpit-modules-sensors-gps \
    -lopenpit-utils-extensions \
    -lopenpit-utils-trdparty-boostdi \
    -lOpenPitUtilsTrdPartyRxQt \

DESTDIR     = $$PWD/../binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/sbin
INSTALLS += target
