include(../../auto.pri)

QT += contacts contacts-private

HEADERS += ../qcontactmanagerdataholder.h

SOURCES  += tst_qcontactmanagerdetails.cpp

INCLUDEPATH += .. \
               ../..
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
