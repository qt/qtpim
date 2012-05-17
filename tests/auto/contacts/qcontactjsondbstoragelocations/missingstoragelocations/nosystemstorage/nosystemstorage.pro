include(../../../../auto.pri)

TARGET = tst_nosystemstorage

QT += contacts
contains(QT_CONFIG, jsondb): QT += jsondb

SOURCES += tst_nosystemstorage.cpp \
           ../tst_missingstorage.cpp \
           ../../../../../../src/plugins/contacts/jsondb/qcontactjsondbid.cpp

HEADERS += ../tst_missingstorage.h \
           ../../../../jsondbprocess.h \
           ../../../../../../src/plugins/contacts/jsondb/qcontactjsondbid.h

INCLUDEPATH += ..
INCLUDEPATH += ../../../..
INCLUDEPATH += ../../../../../../src/plugins/contacts/jsondb

OTHER_FILES += partitions.json
