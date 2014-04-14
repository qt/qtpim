TARGET = qtorganizer_memory
QT = core organizer-private

PLUGIN_TYPE = organizer
load(qt_plugin)

HEADERS += \
    qorganizeritemmemorybackend_p.h

SOURCES += \
    qorganizeritemmemorybackend.cpp

OTHER_FILES += memory.json
