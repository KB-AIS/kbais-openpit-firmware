PRJDIR = $$PWD/..

TARGET = OpenPitUtilsExtensions

TEMPLATE = lib

QT -= qt \

CONFIG += staticlib \

QMAKE_CXXFLAGS += -std=gnu++17 -Wno-psabi \

HEADERS += \
    QtExtensions/QByteArrayExt.h \
    Casting.h \
    Crc16Alogs.h \
    Crc8Alogs.h \
    QtExtensions/QTimerExt.h

SOURCES += \
    Crc16Alogs.cpp \
    Crc8Alogs.cpp \

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
