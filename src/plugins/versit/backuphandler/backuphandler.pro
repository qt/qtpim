load(qt_module)

TARGET = qtversit_backuphandler
QT += versit versit-private contacts

load(qt_plugin)

DESTDIR = $$QT.versit.plugins/versit

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
