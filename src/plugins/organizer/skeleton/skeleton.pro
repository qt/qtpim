TARGET = qtorganizer_skeleton
QT += organizer

PLUGIN_TYPE = organizer
PLUGIN_CLASS_NAME = QSkeletonOrganizerPlugin
load(qt_plugin)

HEADERS += qorganizerskeleton_p.h
SOURCES += qorganizerskeleton.cpp
OTHER_FILES += skeleton.json
