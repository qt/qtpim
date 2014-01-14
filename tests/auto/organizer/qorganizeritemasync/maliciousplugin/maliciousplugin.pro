PLUGIN_TYPE = organizer
load(qt_plugin)

QT += core organizer

DEFINES += MALICIOUSPLUGINTARGET=organizer_maliciousplugin
DEFINES += MALICIOUSPLUGINNAME=maliciousplugin

HEADERS += maliciousplugin_p.h
SOURCES += maliciousplugin.cpp

OTHER_FILES += malicious.json
