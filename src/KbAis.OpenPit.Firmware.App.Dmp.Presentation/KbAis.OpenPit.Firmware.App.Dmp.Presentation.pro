PRJDIR = $$PWD/..

TARGET = OpenPitAppDmpPresentation

QT += core gui widgets

TEMPLATE = lib

CONFIG += \
    c++11 c++14 c++17 \
    depend_includepath \
    conan_basic_setup \

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.Modules.Sensors.Gps \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    MainView.h \
    ViewWrapper.h

SOURCES += \
    MainView.cpp \
    ViewWrapper.cpp \

FORMS += \
    MainView.ui \

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
