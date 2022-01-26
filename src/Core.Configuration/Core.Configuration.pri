CONFIG += 11 14 17

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/Configuration.h \
    $$PWD/ConfigurationChangeSource.h \
    $$PWD/ConfigurationsManager.h \
    $$PWD/IConfigurationProvider.h \
    $$PWD/IRxConfigurationChangePublisher.h

SOURCES += \
    $$PWD/ConfigurationChangeSource.cpp \
    $$PWD/ConfigurationsManager.cpp