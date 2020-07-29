TARGET = qtcontacts_memory
QT = core contacts

PLUGIN_TYPE = contacts
PLUGIN_CLASS_NAME = QMemoryContactsPlugin
load(qt_plugin)

HEADERS += \
    qcontactmemorybackend_p.h

SOURCES += \
    qcontactmemorybackend.cpp

OTHER_FILES += memory.json
