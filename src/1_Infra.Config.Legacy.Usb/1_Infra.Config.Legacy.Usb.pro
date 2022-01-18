PRJDIR = $${PWD}/..

QT += core serialport

CONFIG += 11 14 17 conan_basic_setup

TEMPLATE = lib

include($${PRJDIR}/../conanbuildinfo.pri)
include($${PRJDIR}/2_App.Config.Legacy/2_App.Config.Legacy.pri)
include($${PRJDIR}/TrdParty/TrdParty.pri)

SOURCES += \
    LegacyConfigService.cpp

HEADERS += \
    LegacyConfigService.h
