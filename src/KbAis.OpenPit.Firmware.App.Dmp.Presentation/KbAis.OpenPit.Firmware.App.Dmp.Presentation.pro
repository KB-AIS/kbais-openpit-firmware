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
    opf/app/view/dmp/module_factory_app_view_dmp.h \
    opf/app/view/dmp/nav/nav_controller.h \
    opf/app/view/dmp/nav/ntf_controller.h \
    opf/app/view/dmp/nav/pop_controller.h \
    opf/app/view/dmp/runner_app_view_dmp.h \
    opf/app/view/dmp/screens/host_window.h \
    opf/app/view/dmp/screens/diag/diag_view.h \
    opf/app/view/dmp/screens/info/info_view.h \
    opf/app/view/dmp/screens/main/main_view_dialog_nav.h \
    opf/app/view/dmp/screens/main/main_view.h \
    opf/app/view/dmp/screens/main/subviews/gauge_flv_subview.h \
    opf/app/view/dmp/screens/main/subviews/gauge_spd_subview.h \
    opf/app/view/dmp/screens/main/subviews/gauge_wgt_subview.h \
    opf/app/view/dmp/screens/notificator_w_action.h \
    opf/app/view/dmp/screens/stop/stop_view.h \
    opf/app/view/dmp/screens/stop/stop_view_dialog_cause.h \
    opf/app/view/dmp/state_changed_publisher.h \
    opf/app/view/dmp/utils/neon_colors.h

SOURCES += \
    opf/app/view/dmp/nav/nav_controller.cpp \
    opf/app/view/dmp/nav/ntf_controller.cpp \
    opf/app/view/dmp/nav/pop_controller.cpp \
    opf/app/view/dmp/runner_app_view_dmp.cpp \
    opf/app/view/dmp/screens/host_window.cpp \
    opf/app/view/dmp/screens/diag/diag_view.cpp \
    opf/app/view/dmp/screens/info/info_view.cpp \
    opf/app/view/dmp/screens/main/main_view_dialog_nav.cpp \
    opf/app/view/dmp/screens/main/main_view.cpp \
    opf/app/view/dmp/screens/notificator_w_action.cpp \
    opf/app/view/dmp/screens/stop/stop_view.cpp \
    opf/app/view/dmp/screens/stop/stop_view_dialog_cause.cpp

FORMS += \
    opf/app/view/dmp/screens/host_window.ui \
    opf/app/view/dmp/screens/diag/diag_view.ui \
    opf/app/view/dmp/screens/info/info_view.ui \
    opf/app/view/dmp/screens/main/main_view.ui \
    opf/app/view/dmp/screens/main/main_view_dialog_nav.ui \
    opf/app/view/dmp/screens/notificator_w_action.ui \
    opf/app/view/dmp/screens/stop/stop_view.ui \
    opf/app/view/dmp/screens/stop/stop_view_dialog_cause.ui

RESOURCES += \
    opf/app/view/dmp/resources.qrc

DESTDIR     = $$PRJDIR/binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/lib
INSTALLS += target

DISTFILES += \
    opf/app/view/dmp/res/icons/clip.svg
