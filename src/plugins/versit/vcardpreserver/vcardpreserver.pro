load(qt_module)

QT += versit contacts

TARGET = qtversit_vcardpreserver
target.path += $$[QT_INSTALL_PLUGINS]/versit
INSTALLS += target

load(qt_plugin)
DESTDIR = $$QT.versit.plugins/versit

HEADERS += vcardpreserver.h
SOURCES += vcardpreserver.cpp

OTHER_FILES += vcardpreserver.json
