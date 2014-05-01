include(../../../auto.pri)

QT += contacts versit contacts-private
qtHaveModule(serviceframework) {
    QT += serviceframework
}

SOURCES  += tst_qcontactmanagerfiltering.cpp
HEADERS += ../../qcontactmanagerdataholder.h

INCLUDEPATH += ../.. \
               ../../..

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
