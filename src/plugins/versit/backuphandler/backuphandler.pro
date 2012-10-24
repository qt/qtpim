QT += versit versit-private contacts

TARGET = qtversit_backuphandler

PLUGIN_TYPE = versit
load(qt_plugin)

HEADERS += backupvcardhandler.h
SOURCES += backupvcardhandler.cpp

OTHER_FILES += backuphandler.json
