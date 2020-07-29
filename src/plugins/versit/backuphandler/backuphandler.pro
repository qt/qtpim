TARGET = qtversit_backuphandler
QT += contacts versit-private

PLUGIN_TYPE = versit
PLUGIN_CLASS_NAME = QBackupHandlerVersitPlugin
load(qt_plugin)

HEADERS += backupvcardhandler.h
SOURCES += backupvcardhandler.cpp

OTHER_FILES += backuphandler.json
