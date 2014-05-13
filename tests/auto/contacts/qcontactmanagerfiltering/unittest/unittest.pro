include(../../../auto.pri)

QT += contacts versit contacts-private
qtHaveModule(serviceframework) {
    QT += serviceframework
}

contains(QT_CONFIG,icu):DEFINES += QT_USE_ICU

SOURCES  += tst_qcontactmanagerfiltering.cpp
HEADERS += ../../qcontactmanagerdataholder.h

INCLUDEPATH += ../.. \
               ../../..

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
