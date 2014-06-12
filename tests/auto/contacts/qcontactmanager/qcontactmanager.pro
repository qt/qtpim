include(../../auto.pri)

QT += contacts versit contacts-private

SOURCES  += tst_qcontactmanager.cpp
HEADERS += ../qcontactmanagerdataholder.h
HEADERS += ../qcontactidmock.h

OTHER_FILES += lazy.json

INCLUDEPATH += .. \
               ../..
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
