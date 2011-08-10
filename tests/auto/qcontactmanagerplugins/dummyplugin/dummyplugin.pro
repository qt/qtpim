TEMPLATE = lib
CONFIG += plugin testplugin
TARGET = $$qtLibraryTarget(contacts_testdummy)
PLUGIN_TYPE=contacts

DESTDIR = $$QT.contacts.plugins/contacts
QT += contacts

SOURCES += dummyplugin.cpp
HEADERS += dummyplugin.h

DEFINES += DUMMYPLUGINNAME=testdummy

include(../../contacts_plugins.pri)

