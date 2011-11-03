TEMPLATE = lib
CONFIG += qt warn_on depend_includepath plugin
DESTDIR = $$QT.contacts.plugins/contacts

qtAddLibrary(QtTest)

QT += contacts

DEFINES += MALICIOUSPLUGINTARGET=contacts_maliciousplugin
DEFINES += MALICIOUSPLUGINNAME=maliciousplugin

HEADERS += maliciousplugin_p.h
SOURCES += maliciousplugin.cpp

# Enable installation of plugin in "make install"
TARGET = maliciousplugin
target.path += $$[QT_INSTALL_PLUGINS]/contacts
INSTALLS += target
