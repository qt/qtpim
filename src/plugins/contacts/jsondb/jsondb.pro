load(qt_module)

QT += contacts contacts-private network declarative jsondb

contains(QT_CONFIG, jsondbcompat): {
    QT -= jsondb
    QT += jsondbcompat
}

TARGET = qtcontacts_jsondb
target.path += $$[QT_INSTALL_PLUGINS]/contacts
INSTALLS += target

load(qt_plugin)

DESTDIR = $$QT.contacts.plugins/contacts

# Input
HEADERS += qcontactjsondbenginefactory.h \
    qcontactjsondbengine.h \
    qcontactjsondbenginedata.h \
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
