PRJDIR = $$PWD/..

TARGET = openpit-utils-trdparty-boostdi

QT -= core gui

TEMPLATE = lib

CONFIG += \
    c++11 c++14 c++17 \
    depend_includepath \
    conan_basic_setup \

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
