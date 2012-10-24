QT = core contacts

TARGET = qtcontacts_memory

PLUGIN_TYPE = contacts
load(qt_plugin)

HEADERS += \
    qcontactmemorybackend_p.h

SOURCES += \
    qcontactmemorybackend.cpp

OTHER_FILES += memory.json
