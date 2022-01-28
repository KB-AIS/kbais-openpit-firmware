PRJDIR = $$PWD/..

TARGET = OpenPitCoreConfiguration

TEMPLATE = lib

QMAKE_CXXFLAGS += -Wno-psabi

CONFIG += 11 14 17 conan_basic_setup

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.RxQt \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    Configuration.h \
    ConfigurationChangeSource.h \
    ConfigurationsManager.h \
    IConfigurationProvider.h \
    IRxConfigurationChangePublisher.h \

SOURCES += \
    ConfigurationChangeSource.cpp \
    ConfigurationsManager.cpp \

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target
