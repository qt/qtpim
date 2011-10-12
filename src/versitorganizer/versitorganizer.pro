load(qt_module)

TARGET = QtAddOnVersitOrganizer
QPRO_PWD = $PWD

CONFIG += module
MODULE_PRI = ../../modules/qt_versitorganizer.pri

QT = core versit versit-private organizer

DEFINES += QT_BUILD_VERSITORGANIZER_LIB QT_MAKEDLL

load(qt_module_config)
VERSION = $$QT.versitorganizer.VERSION

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

HEADERS += qtaddonversitorganizerversion.h $$PUBLIC_HEADERS $$PRIVATE_HEADERS
