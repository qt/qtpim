include(../../../../auto.pri)

TARGET = tst_nouserdatastorage

QT += contacts
qtHaveModule(jsondb): QT += jsondb

SOURCES += tst_nouserdatastorage.cpp \
           ../tst_missingstorage.cpp \
           ../../../../../../src/plugins/contacts/jsondb/qcontactjsondbid.cpp

HEADERS += ../tst_missingstorage.h \
           ../../../../jsondbprocess.h \
           ../../../../../../src/plugins/contacts/jsondb/qcontactjsondbid.h

INCLUDEPATH += ..
INCLUDEPATH += ../../../..
INCLUDEPATH += ../../../../../../src/plugins/contacts/jsondb

OTHER_FILES += partitions.json
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
