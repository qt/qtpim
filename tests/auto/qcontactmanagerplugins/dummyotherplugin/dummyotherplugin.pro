TEMPLATE = lib
CONFIG += plugin testplugin
TARGET = $$qtLibraryTarget(contacts_testotherdummy)
PLUGIN_TYPE=contacts
DESTDIR = $$QT.contacts.plugins/contacts
QT += contacts

SOURCES += dummyotherplugin.cpp
    
include(../../contacts_plugins.pri)
