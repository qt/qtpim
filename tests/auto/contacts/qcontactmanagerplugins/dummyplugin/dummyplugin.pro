TEMPLATE = lib
CONFIG += plugin testplugin
TARGET = $$qtLibraryTarget(contacts_testdummy)
PLUGIN_TYPE=contacts

DESTDIR = $$QT.contacts.plugins/contacts
QT += contacts

SOURCES += dummyplugin.cpp
HEADERS += dummyplugin.h

DEFINES += DUMMYPLUGINNAME=testdummy

# Enable installation of plugin in "make install"
target.path += $$[QT_INSTALL_PLUGINS]/contacts
INSTALLS += target
OTHER_FILES += dummy.json
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
