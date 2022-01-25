PRJDIR = $$PWD/..

TEMPLATE = lib

CONFIG += 11 14 17 conan_basic_setup

include($$PRJDIR/../conanbuildinfo.pri)
include($$PRJDIR/TrdParty/TrdParty.pri)

HEADERS += \
    ConfigurationService.h

SOURCES += \
    ConfigurationService.cpp
