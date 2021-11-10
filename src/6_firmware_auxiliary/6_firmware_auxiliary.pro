TARGET = carrier-auxiliary

QT += core gui widgets

CONFIG += c++17

INCLUDEPATH += \
    # project
    ../0_sensors_gps \
    # oss
    ../_oss/plog/include

SOURCES += \
    host_wrapper.cpp \
    main.cpp \
    presentation/diag_presenter.cpp \
    presentation/main_presenter.cpp

HEADERS += \
    host_wrapper.h \
    presentation/diag_presenter.h \
    presentation/main_presenter.h

FORMS += \
    presentation/diag_view.ui \
    presentation/main_view.ui

LIBS += -L$$PWD/../binaries \
    -lsensors-gps \
    -lutils

message(6_firmware_auxiliary project dir: $${PWD})

DESTDIR     = $$PWD/../binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

message(6_firmware_auxiliary output dir: $${DESTDIR})

target.path += /usr/sbin
INSTALLS += target
