TARGET = qtversit_vcardpreserver
QT += contacts versit

PLUGIN_TYPE = versit
load(qt_plugin)

HEADERS += vcardpreserver.h
SOURCES += vcardpreserver.cpp

OTHER_FILES += vcardpreserver.json
