include(../../auto.pri)

QT += contacts contacts-private
qtHaveModule(jsondb) {
    QT += jsondb
}

HEADERS += ../qcontactmanagerdataholder.h

SOURCES  += tst_qcontactmanagerdetails.cpp

INCLUDEPATH += .. \
               ../..
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
