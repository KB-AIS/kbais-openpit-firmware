QT += core gui widgets

CONFIG += c++17

TARGET = carrier-auxiliary

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
    host_presenter.cpp \
    main.cpp \
    presentation/diag_presenter.cpp \
    presentation/main_presenter.cpp

HEADERS += \
    host_presenter.h \
    presentation/diag_presenter.h \
    presentation/main_presenter.h

FORMS += \
    presentation/diag_view.ui \
    presentation/main_view.ui

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
