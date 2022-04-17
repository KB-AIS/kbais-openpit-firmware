PRJDIR = $$PWD/..

TARGET = OpenPitCoreConfiguration

TEMPLATE = lib

QT += \

CONFIG += staticlib conan_basic_setup \

QMAKE_CXXFLAGS += -std=gnu++17 -Wno-psabi \

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.RxQt \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    core/configuration/app_configuration.h \
    core/configuration/app_configuration_manager.h \
    core/configuration/app_configuration_watcher.h

SOURCES += \
    core/configuration/app_configuration_manager.cpp \
    core/configuration/app_configuration_watcher.cpp

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
