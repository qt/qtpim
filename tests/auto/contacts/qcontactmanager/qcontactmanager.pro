include(../../auto.pri)

QT += contacts versit contacts-private jsondb

SOURCES  += tst_qcontactmanager.cpp
HEADERS += ../qcontactmanagerdataholder.h
HEADERS += ../../jsondbprocess.h \
           ../qcontactidmock.h

OTHER_FILES += lazy.json

INCLUDEPATH += .. \
               ../..
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
