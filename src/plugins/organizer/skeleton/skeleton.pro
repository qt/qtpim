load(qt_build_config)

QT += organizer

TARGET = qtorganizer_skeleton
target.path += $$[QT_INSTALL_PLUGINS]/organizer
INSTALLS += target

load(qt_plugin)

DESTDIR = $$QT.organizer.plugins/organizer

HEADERS += qorganizerskeleton_p.h
SOURCES += qorganizerskeleton.cpp
OTHER_FILES += skeleton.json
