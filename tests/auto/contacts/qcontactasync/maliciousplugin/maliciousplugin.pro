PLUGIN_TYPE = contacts
load(qt_plugin)

QT += contacts testlib

DEFINES += MALICIOUSPLUGINTARGET=contacts_maliciousplugin
DEFINES += MALICIOUSPLUGINNAME=maliciousplugin

HEADERS += maliciousplugin_p.h
SOURCES += maliciousplugin.cpp

OTHER_FILES += \
    malicious.json
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
