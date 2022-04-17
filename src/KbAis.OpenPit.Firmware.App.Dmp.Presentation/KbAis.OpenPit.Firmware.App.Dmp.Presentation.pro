PRJDIR = $$PWD/..

TARGET = OpenPitAppDmpPresentation

TEMPLATE = lib

QT += core gui widgets serialport svg \

CONFIG += staticlib depend_includepath conan_basic_setup \

QMAKE_CXXFLAGS += -std=gnu++17 -Wno-psabi \

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Configuration \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Networking \
    $$PRJDIR/KbAis.OpenPit.Firmware.Modules.Sensors.Gps \
    $$PRJDIR/KbAis.OpenPit.Firmware.Modules.Sensors.Serials \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.RxQt \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.Widgets \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    App/Dmp/Presentation/BootstrapperPresentation.h \
    App/Dmp/Presentation/HostWindow.h \
    App/Dmp/Presentation/ModuleFactoryPresentation.h \
    App/Dmp/Presentation/Utils/NavController.h \
    App/Dmp/Presentation/Views/Diag/DiagView.h \
    App/Dmp/Presentation/Views/Main/SetupGaugeSpeed.h \
    App/Dmp/Presentation/Views/Main/main_view.h \
    App/Dmp/Presentation/Views/Main/main_weight_view.h \
    presentation/dmp/state_changed_publisher.h

SOURCES += \
    App/Dmp/Presentation/BootstrapperPresentation.cpp \
    App/Dmp/Presentation/HostWindow.cpp \
    App/Dmp/Presentation/Utils/NavController.cpp \
    App/Dmp/Presentation/Views/Diag/DiagView.cpp \
    App/Dmp/Presentation/Views/Main/SetupGaugeSpeed.cpp \
    App/Dmp/Presentation/Views/Main/main_view.cpp \
    App/Dmp/Presentation/Views/Main/main_weight_view.cpp

FORMS += \
    App/Dmp/Presentation/HostWindow.ui \
    App/Dmp/Presentation/Views/Diag/DiagView.ui \
    App/Dmp/Presentation/Views/Main/main_view.ui

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target

RESOURCES += \
    opf_dmp.qrc
