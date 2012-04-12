load(qt_module)

TARGET = QtVersitOrganizer
QPRO_PWD = $PWD

CONFIG += module

QT = core versit-private organizer

DEFINES += QT_BUILD_VERSITORGANIZER_LIB QT_MAKEDLL

load(qt_module_config)

PUBLIC_HEADERS += \
    qversitorganizerglobal.h \
    qversitorganizerhandler.h \
    qversitorganizerexporter.h \
    qversitorganizerimporter.h \
    qversittimezonehandler.h

PRIVATE_HEADERS += \
    qversitorganizerexporter_p.h \
    qversitorganizerimporter_p.h \
    qversitorganizerdefs_p.h \
    qversitorganizerpluginloader_p.h \
    qtimezones_p.h \
    qversitorganizerpluginsearch_p.h

SOURCES += \
    qversitorganizerexporter.cpp \
    qversitorganizerexporter_p.cpp \
    qversitorganizerimporter.cpp \
    qversitorganizerimporter_p.cpp \
    qversitorganizerhandler.cpp \
    qversitorganizerpluginloader_p.cpp \
    qtimezones_p.cpp

HEADERS += qtversitorganizerversion.h $$PUBLIC_HEADERS $$PRIVATE_HEADERS
