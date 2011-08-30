TEMPLATE = lib
CONFIG += plugin
TARGET = $$qtLibraryTarget(qtversit_backuphandler)
DESTDIR = $$QT.versit.plugins/versit
QT += versit versit-private contacts

HEADERS += backupvcardhandler.h
SOURCES += backupvcardhandler.cpp

#symbian {
#    TARGET.EPOCALLOWDLLDATA = 1
#    TARGET.CAPABILITY = ALL -Tcb
#    TARGET.UID3 = 0x20031600

#    target.path = /sys/bin
#    INSTALLS += target

#    symbianplugin.sources = $${TARGET}.dll
#    symbianplugin.path = $${QT_PLUGINS_BASE_DIR}/$${PLUGIN_TYPE}
#    DEPLOYMENT += symbianplugin
#}
