TARGET = qtcontacts_memory
QT = core contacts

PLUGIN_TYPE = contacts
load(qt_plugin)

HEADERS += \
    qcontactmemorybackend_p.h

SOURCES += \
    qcontactmemorybackend.cpp

OTHER_FILES += memory.json
