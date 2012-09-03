QT = core contacts

TARGET = qtcontacts_memory
target.path += $$[QT_INSTALL_PLUGINS]/contacts
INSTALLS += target

load(qt_plugin)

DESTDIR = $$QT.contacts.plugins/contacts

HEADERS += \
    qcontactmemorybackend_p.h

SOURCES += \
    qcontactmemorybackend.cpp

OTHER_FILES += memory.json
