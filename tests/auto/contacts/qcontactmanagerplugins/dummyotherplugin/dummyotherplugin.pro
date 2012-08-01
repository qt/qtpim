TEMPLATE = lib
CONFIG += plugin testplugin
TARGET = $$qtLibraryTarget(contacts_testotherdummy)
PLUGIN_TYPE=contacts
DESTDIR = $$QT.contacts.plugins/contacts
QT += contacts

SOURCES += dummyotherplugin.cpp

# Enable installation of plugin in "make install"
target.path += $$[QT_INSTALL_PLUGINS]/contacts
INSTALLS += target
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
