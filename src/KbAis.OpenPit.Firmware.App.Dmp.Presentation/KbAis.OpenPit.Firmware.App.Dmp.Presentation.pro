PRJDIR = $$PWD/..

TARGET = OpenPitAppDmpPresentation

TEMPLATE = lib

QT += core gui widgets \

CONFIG += staticlib depend_includepath conan_basic_setup \

QMAKE_CXXFLAGS += -std=gnu++17 -Wno-psabi \

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.Modules.Sensors.Gps \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Networking \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.RxQt \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    Diag/DiagView.h \
    HostView.h \
    MainView.h \
    NavigationEmmiter.h \
    ViewWrapper.h

SOURCES += \
    Diag/DiagView.cpp \
    HostView.cpp \
    MainView.cpp \
    NavigationEmmiter.cpp \
    ViewWrapper.cpp \

FORMS += \
    Diag/DiagView.ui \
    HostView.ui \
    MainView.ui \

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
