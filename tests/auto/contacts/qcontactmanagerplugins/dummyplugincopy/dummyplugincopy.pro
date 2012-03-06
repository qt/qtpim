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

# Enable installation of plugin in "make install"
target.path += $$[QT_INSTALL_PLUGINS]/contacts
INSTALLS += target
