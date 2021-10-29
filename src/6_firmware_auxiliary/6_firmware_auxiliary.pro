QT += core gui widgets

TARGET = carrier-auxiliary

CONFIG += c++17

INCLUDEPATH += \
    # SENSORS
    # GPS sensor
    ../0_sensors_gps \
    # OSS
    # plog -- logging library
    ../_oss/plog/include

DEPENDPATH += \
    ../0_sensors_gps

SOURCES += \
    forms/main_form.cpp \
    main.cpp

HEADERS += \
    forms/main_form.h

FORMS += \
    forms/main_form.ui

LIBS += -L$$PWD/../binaries \
    -lsensors-gps

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
