QT += testlib
TEMPLATE=app
TARGET=tst_qcontactactions
CONFIG+=testcase

PLUGIN_SUBDIR=dummyplugin/plugins

include(../../../../common.pri)

INCLUDEPATH += ../../../../src/contacts \
               ../../../../src/contacts/details \
               ../../../../src/contacts/requests \
               ../../../../src/contacts/filters \
               ../../../../src/serviceframework

INCLUDEPATH += ../../

CONFIG += mobility
MOBILITY = contacts serviceframework
SOURCES  += tst_qcontactactions.cpp

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
