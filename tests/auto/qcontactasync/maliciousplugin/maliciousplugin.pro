TEMPLATE = lib
CONFIG += qt debug warn_on depend_includepath plugin
DESTDIR = $$QT.contacts.plugins/contacts

qtAddLibrary(QtTest)

QMAKE_LIBS += -Wl,-rpath,$${QT.contacts.libs}

QT += contacts

DEFINES += MALICIOUSPLUGINTARGET=contacts_maliciousplugin
DEFINES += MALICIOUSPLUGINNAME=maliciousplugin

HEADERS += maliciousplugin_p.h
SOURCES += maliciousplugin.cpp
