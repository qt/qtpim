TARGET = qtorganizer_memory
QT = core organizer-private

PLUGIN_TYPE = organizer
PLUGIN_CLASS_NAME = QMemoryOrganizerPlugin
load(qt_plugin)

HEADERS += \
    qorganizeritemmemorybackend_p.h

SOURCES += \
    qorganizeritemmemorybackend.cpp

OTHER_FILES += memory.json
