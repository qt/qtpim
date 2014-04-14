TARGET = qtorganizer_skeleton
QT += organizer

PLUGIN_TYPE = organizer
load(qt_plugin)

HEADERS += qorganizerskeleton_p.h
SOURCES += qorganizerskeleton.cpp
OTHER_FILES += skeleton.json
