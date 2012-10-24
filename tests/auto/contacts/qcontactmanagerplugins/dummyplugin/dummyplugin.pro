CONFIG += testplugin
TARGET = contacts_testdummy
PLUGIN_TYPE=contacts
load(qt_plugin)

QT += contacts

SOURCES += dummyplugin.cpp
HEADERS += dummyplugin.h

DEFINES += DUMMYPLUGINNAME=testdummy

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
