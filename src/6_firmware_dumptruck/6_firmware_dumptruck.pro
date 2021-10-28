CONFIG += c++17

SOURCES += \
    main.cpp

message(6_firmware_dumptruck project dir: $${PWD})

# Artifact direcotires configuration
DESTDIR     = $$PWD/../binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

message(6_firmware_dumptruck output dir: $${DESTDIR})
