PRJDIR = $$PWD/..

TARGET = OpenPitUtilsTrdPartyJsonQt

QT -= qt

TEMPLATE = lib

CONFIG += c++14 conan_basic_setup

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    JsonQt.h \

DESTDIR     = $$PWD/../binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target