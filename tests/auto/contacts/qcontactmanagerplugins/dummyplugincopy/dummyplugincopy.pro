CONFIG += testplugin
TARGET = contacts_testdummycopy
PLUGIN_TYPE=contacts
load(qt_plugin)

QT += contacts

DEFINES += DUMMYPLUGINTARGET=contacts_testdummycopy
DEFINES += DUMMYPLUGINNAME=testdummy

SOURCES += ../dummyplugin/dummyplugin.cpp
HEADERS += ../dummyplugin/dummyplugin.h

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
