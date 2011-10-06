include(../auto.pri)

QT += contacts contacts-private jsondb jsondb-private

SOURCES += tst_qcontactjsondbconverter.cpp \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbconverter.cpp \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbenginefactory.cpp \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbengine.cpp \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbrequesthandler.cpp \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbrequestmanager.cpp

HEADERS += ../../../src/plugins/contacts/jsondb/qcontactjsondbenginefactory.h \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbengine.h \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbconverter.h \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbrequesthandler.h \
           ../../../src/plugins/contacts/jsondb/qcontactjsondbrequestmanager.h

INCLUDEPATH += ../../../src/plugins/contacts/jsondb
