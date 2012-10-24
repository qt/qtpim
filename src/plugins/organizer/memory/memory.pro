QT = core organizer organizer-private

TARGET = qtorganizer_memory

PLUGIN_TYPE = organizer
load(qt_plugin)

HEADERS += \
    qorganizeritemmemorybackend_p.h

SOURCES += \
    qorganizeritemmemorybackend.cpp

OTHER_FILES += memory.json
