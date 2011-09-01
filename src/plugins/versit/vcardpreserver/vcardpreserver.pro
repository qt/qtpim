load(qt_module)

QT += versit contacts

TARGET = qtversit_vcardpreserver
target.path += $$[QT_INSTALL_PLUGINS]/versit
INSTALLS += target

load(qt_plugin)
DESTDIR = $$QT.versit.plugins/versit

HEADERS += vcardpreserver.h
SOURCES += vcardpreserver.cpp

#symbian {
#    TARGET.EPOCALLOWDLLDATA = 1
#    TARGET.CAPABILITY = ALL -Tcb
#    TARGET.UID3 = 0x20031602

#    target.path = /sys/bin
#    INSTALLS += target

#    symbianplugin.sources = $${TARGET}.dll
#    symbianplugin.path = $${QT_PLUGINS_BASE_DIR}/$${PLUGIN_TYPE}
#    DEPLOYMENT += symbianplugin
#}
