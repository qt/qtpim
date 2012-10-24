CONFIG += testplugin
TARGET = contacts_testotherdummy
PLUGIN_TYPE=contacts
load(qt_plugin)
QT += contacts

SOURCES += dummyotherplugin.cpp

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
