PRJDIR = $$PWD/..

TEMPLATE = lib

CONFIG += 11 14 17 conan_basic_setup

include($$PRJDIR/../conanbuildinfo.pri)
include($$PRJDIR/TrdParty/TrdParty.pri)

HEADERS += \
    Configuration.h \
    ConfigurationChangeSource.h \
    ConfigurationsManager.h \
    IConfigurationProvider.h \
    IRxConfigurationChangePublisher.h

SOURCES += \
    ConfigurationChangeSource.cpp \
    ConfigurationsManager.cpp
