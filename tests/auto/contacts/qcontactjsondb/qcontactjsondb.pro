include(../../auto.pri)

QT += contacts contacts-private jsondb

contains(QT_CONFIG, jsondbcompat): {
    QT -= jsondb
    QT += jsondbcompat
}

SOURCES += tst_qcontactjsondbengine.cpp \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbenginefactory.cpp \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbengine.cpp \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbrequesthandler.cpp \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbrequestmanager.cpp \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbconverter.cpp \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbstring.cpp \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbid.cpp \
           qcontactjsondbbackup.cpp \
           synchronizedjsondbclient.cpp \
           synchronizedworker.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../../src/plugins/contacts/jsondb

HEADERS += qcontactjsondbbackup.h \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbenginefactory.h \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbengine.h \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbenginedata.h \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbrequesthandler.h \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbrequestmanager.h \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbconverter.h \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbstring.h \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbid.h \
           synchronizedjsondbclient.h \
           synchronizedworker.h
