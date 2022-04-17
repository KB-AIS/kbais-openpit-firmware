PRJDIR = $$PWD/..

TARGET = opf

TEMPLATE = app

QT += core gui network serialport sql widgets svg \

CONFIG += depend_includepath conan_basic_setup \

QMAKE_CXXFLAGS += -std=gnu++17 -Wno-psabi -faligned-new \

INCLUDEPATH += \
    $$PRJDIR/KbAis.OpenPit.Firmware.App.Dmp.Presentation \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Configuration \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.EventBus \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Messaging \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Networking \
    $$PRJDIR/KbAis.OpenPit.Firmware.Core.Persisting \
    $$PRJDIR/KbAis.OpenPit.Firmware.Modules.Legacy.Agtp \
    $$PRJDIR/KbAis.OpenPit.Firmware.Modules.Sensors.Gps \
    $$PRJDIR/KbAis.OpenPit.Firmware.Modules.Sensors.Serials \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.Extensions \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.BoostDi \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.JsonQt \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.TrdParty.RxQt \
    $$PRJDIR/KbAis.OpenPit.Firmware.Utils.Widgets \

include($$PRJDIR/../conanbuildinfo.pri)

HEADERS += \
    app_configuration_initializer_dmp.h \
    composition_root_factory.h \
    conifgurator_cli_processor.h \
    messaging/immediate_message_mapper_dmp.h \
    messaging/recurrent_message_mapper_dmp.h \
    simulating/fake_gps_sensor_publisher.h \
    simulating/fake_lcs_sensor_publisher.h \
    simulating/scenario_executor.h \
    state_watching/module_factory_state_watchers.h \
    state_watching/runner_state_watcher.h \
    state_watching/shift_watcher.h \
    state_watching/state_watcher.h \
    state_watching/state_watcher_fuelling.h \
    state_watching/state_watcher_loading.h \
    state_watching/state_watcher_motioning.h \
    system/module_factory_system_services.h \
    system/setup_datetime_service.h \
    system/setup_ethernet_service.h

SOURCES += \
    app_configuration_initializer_dmp.cpp \
    conifgurator_cli_processor.cpp \
    main.cpp \
    messaging/immediate_message_mapper_dmp.cpp \
    messaging/recurrent_message_mapper_dmp.cpp \
    simulating/fake_gps_sensor_publisher.cpp \
    simulating/fake_lcs_sensor_publisher.cpp \
    state_watching/runner_state_watcher.cpp \
    state_watching/shift_watcher.cpp \
    state_watching/state_watcher_fuelling.cpp \
    state_watching/state_watcher_loading.cpp \
    state_watching/state_watcher_motioning.cpp \
    system/setup_datetime_service.cpp \
    system/setup_ethernet_service.cpp

LIBS += -L$$PRJDIR/binaries \
    -lOpenPitAppDmpPresentation \
    -lOpenPitCoreConfiguration \
    -lOpenPitCoreEventBus \
    -lOpenPitCoreMessaging \
    -lOpenPitCoreNetworking \
    -lOpenPitCorePersisting \
    -lOpenPitModulesLegacyAgtp \
    -lOpenPitModulesSensorsGps \
    -lOpenPitModulesSensorsSerials \
    -lOpenPitUtilsExtensions \
    -lOpenPitUtilsTrdPartyBoostDi \
    -lOpenPitUtilsTrdPartyJsonQt \
    -lOpenPitUtilsTrdPartyRxQt \
    -lOpenPitAppDmpUtilsWidgets \

DESTDIR     = $$PWD/../binaries
OBJECTS_DIR = $$PWD/build/.obj
MOC_DIR     = $$PWD/build/.moc
RCC_DIR     = $$PWD/build/.qrc
UI_DIR      = $$PWD/build/.ui

target.path += /usr/sbin
INSTALLS += target
