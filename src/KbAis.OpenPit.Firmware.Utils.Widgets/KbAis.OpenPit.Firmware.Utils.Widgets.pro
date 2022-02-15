PRJDIR = $$PWD/..

TARGET = OpenPitAppDmpUtilsWidgets

TEMPLATE = lib

QT += core gui widgets \

CONFIG += staticlib \

QMAKE_CXXFLAGS += -std=gnu++17 -Wno-psabi \

HEADERS += \
    Utils/Widgets/QcGauge/QcGaugeWidget.h \

SOURCES += \
    Utils/Widgets/QcGauge/QcGaugeWidget.cpp \

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
