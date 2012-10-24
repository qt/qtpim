QT += contacts contacts-private jsondb

TARGET = qtcontacts_jsondb

PLUGIN_TYPE = contacts
load(qt_plugin)

# Input
HEADERS += qcontactjsondbenginefactory.h \
    qcontactjsondbengine.h \
    qcontactjsondbrequesthandler.h \
    qcontactjsondbrequestmanager.h \
    qcontactjsondbconverter.h \
    qcontactjsondbglobal.h \
    qcontactjsondbid.h \
    qcontactjsondbstring.h

SOURCES += qcontactjsondbenginefactory.cpp \
    qcontactjsondbengine.cpp \
    qcontactjsondbrequesthandler.cpp \
    qcontactjsondbrequestmanager.cpp \
    qcontactjsondbconverter.cpp \
    qcontactjsondbid.cpp \
    qcontactjsondbstring.cpp

OTHER_FILES += jsondb.json
