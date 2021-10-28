QT += core gui widgets

TARGET = carrier-auxiliary

CONFIG += c++17

INCLUDEPATH += \
    widgets \
    ../0_sensors_gps

SOURCES += \
    widgets/host_widget.cpp \
    main.cpp

HEADERS += \
    widgets/host_widget.h

FORMS += \
    widgets/host_widget.ui

LIBS += -L$$PWD/../binaries -lsensors-gps

message(6_firmware_auxiliary project dir: $${PWD})

# Artifact direcotires configuration
DESTDIR     = $$PWD/../binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

message(6_firmware_auxiliary output dir: $${DESTDIR})

target.path += /usr/sbin
INSTALLS += target
