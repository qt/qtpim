CONFIG += testplugin
TARGET = contacts_testdummyempty
PLUGIN_TYPE=contacts
load(qt_plugin)

QT += contacts

DEFINES += DUMMYPLUGINTARGET=contacts_testdummyempty

SOURCES += ../dummyplugin/dummyplugin.cpp
HEADERS += ../dummyplugin/dummyplugin.h

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
