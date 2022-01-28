PRJDIR = $$PWD/..

TARGET = OpenPitCoreEventBus

TEMPLATE = lib

QT += core

QMAKE_CXXFLAGS += -Wno-psabi

CONFIG += 11 14 17 conan_basic_setup

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    RxEventBus.h \

SOURCES += \
    RxEventBus.cpp \

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
