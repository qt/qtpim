include(../../auto.pri)

QT += contacts contacts-private jsondb

SOURCES += tst_qcontactjsondbconverter.cpp \
           ../../../../src/plugins/contacts/jsondb/qcontactjsondbconverter.cpp \
           ../../../../src/plugins/contacts/jsondb/qcontactjsondbenginefactory.cpp \
           ../../../../src/plugins/contacts/jsondb/qcontactjsondbengine.cpp \
           ../../../../src/plugins/contacts/jsondb/qcontactjsondbrequesthandler.cpp \
           ../../../../src/plugins/contacts/jsondb/qcontactjsondbrequestmanager.cpp \
           ../../../../src/plugins/contacts/jsondb/qcontactjsondbid.cpp \
           ../../../../src/plugins/contacts/jsondb/qcontactjsondbstring.cpp

HEADERS += ../../../../src/plugins/contacts/jsondb/qcontactjsondbenginefactory.h \
           ../../../../src/plugins/contacts/jsondb/qcontactjsondbengine.h \
           ../../../../src/plugins/contacts/jsondb/qcontactjsondbconverter.h \
           ../../../../src/plugins/contacts/jsondb/qcontactjsondbrequesthandler.h \
           ../../../../src/plugins/contacts/jsondb/qcontactjsondbrequestmanager.h \
           ../../../../src/plugins/contacts/jsondb/qcontactjsondbid.h \
           ../../../../src/plugins/contacts/jsondb/qcontactjsondbstring.h

INCLUDEPATH += ../../../../src/plugins/contacts/jsondb

