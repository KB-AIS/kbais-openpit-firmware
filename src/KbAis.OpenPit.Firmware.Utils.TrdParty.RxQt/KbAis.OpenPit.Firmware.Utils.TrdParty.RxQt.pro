PRJDIR = $$PWD/..

TARGET = OpenPitUtilsTrdPartyRxQt

TEMPLATE = lib

QT -= qt gui \

CONFIG += staticlib conan_basic_setup \

QMAKE_CXXFLAGS += -std=gnu++17 -Wno-psabi \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    Format.h \
    RxQtEvent.h \
    RxQtRunLoop.h \
    RxQtRunLoopThread.h \
    RxQtSignal.h \
    RxQtUtil.h \
    rxqt.h

DESTDIR     = $$PWD/../binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
