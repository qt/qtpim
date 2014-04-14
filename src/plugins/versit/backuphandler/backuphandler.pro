TARGET = qtversit_backuphandler
QT += contacts versit-private

PLUGIN_TYPE = versit
load(qt_plugin)

HEADERS += backupvcardhandler.h
SOURCES += backupvcardhandler.cpp

OTHER_FILES += backuphandler.json
