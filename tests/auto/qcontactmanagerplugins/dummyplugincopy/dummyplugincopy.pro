TEMPLATE = lib
CONFIG += plugin testplugin
TARGET = $$qtLibraryTarget(contacts_testdummycopy)
PLUGIN_TYPE=contacts

DESTDIR = $$QT.contacts.plugins/contacts
QT += contacts

DEFINES += DUMMYPLUGINTARGET=contacts_testdummycopy
DEFINES += DUMMYPLUGINNAME=testdummy

SOURCES += ../dummyplugin/dummyplugin.cpp
HEADERS += ../dummyplugin/dummyplugin.h

include(../../contacts_plugins.pri)