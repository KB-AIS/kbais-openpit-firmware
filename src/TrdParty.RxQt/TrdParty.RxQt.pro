TARGET = trdparty_rxqt

QT -= qt

TEMPLATE = lib

CONFIG += c++14 c++17 conan_basic_setup

include($${PWD}/../../conanbuildinfo.pri)

HEADERS += \
    RxQt.h \
    RxQtEvent.h \
    RxQtRunLoop.h \
    RxQtRunLoopThread.h \
    RxQtSignal.h \
    RxQtUtil.h

DESTDIR     = $${PWD}/../binaries
OBJECTS_DIR = $${PWD}/build/.obj
MOC_DIR     = $${PWD}/build/.moc
RCC_DIR     = $${PWD}/build/.qrc
UI_DIR      = $${PWD}/build/.ui

target.path += /usr/lib
INSTALLS += target
