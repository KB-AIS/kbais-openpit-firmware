PRJDIR = $$PWD/..

TARGET = OpenPitUtilsTrdPartyBoostDi

TEMPLATE = lib

QT -= qt core gui \

QMAKE_CXXFLAGS += -std=gnu++17 -Wno-psabi \

CONFIG += staticlib depend_includepath conan_basic_setup \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    BoostDiExtensions.h \

DESTDIR     = $$PWD/../binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
