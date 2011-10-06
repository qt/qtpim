load(qt_module)

TARGET = QtAddOnContacts
QPRO_PWD = $PWD

CONFIG += module
MODULE_PRI = ../../modules/qt_contacts.pri

QT = core gui

DEFINES += QT_BUILD_CONTACTS_LIB QT_MAKEDLL

load(qt_module_config)
VERSION = $$QT.contacts.VERSION

include(details/details.pri)
include(engines/engines.pri)
include(filters/filters.pri)
include(requests/requests.pri)

PUBLIC_HEADERS += \
    qcontact.h \
    qcontactabstractrequest.h \
    qcontactaction.h \
    qcontactactiondescriptor.h \
    qcontactactionfactory.h \
    qcontactactiontarget.h \
    qcontactchangeset.h \
    qcontactdetail.h \
    qcontactdetaildefinition.h \
    qcontactdetailfielddefinition.h \
    qcontactfetchhint.h \
    qcontactfilter.h \
    qcontactid.h \
    qcontactmanager.h \
    qcontactmanagerengine.h \
    qcontactmanagerenginefactory.h \
    qcontactobserver.h \
    qcontactrelationship.h \
    qcontactsortorder.h \
    qcontactsglobal.h \
    qcontacts.h \
    qlatin1constant.h \

PRIVATE_HEADERS += \
    qcontact_p.h \
    qcontactabstractrequest_p.h \
    qcontactactiondescriptor_p.h \
    qcontactactionmanager_p.h \
    qcontactactiontarget_p.h \
    qcontactchangeset_p.h \
    qcontactdetail_p.h \
    qcontactdetaildefinition_p.h \
    qcontactdetailfielddefinition_p.h \
    qcontactfetchhint_p.h \
    qcontactfilter_p.h \
    qcontactid_p.h \
    qcontactmanager_p.h \
    qcontactmanagerenginev2wrapper_p.h \
    qcontactrelationship_p.h \
    qcontactsortorder_p.h \
    qcontactspluginsearch_p.h

SOURCES += \
    qcontact.cpp \
    qcontactabstractrequest.cpp \
    qcontactaction.cpp \
    qcontactactiondescriptor.cpp \
    qcontactactionfactory.cpp \
    qcontactactionmanager_p.cpp \
    qcontactactiontarget.cpp \
    qcontactchangeset.cpp \
    qcontactdetail.cpp \
    qcontactdetaildefinition.cpp \
    qcontactdetailfielddefinition.cpp \
    qcontactfetchhint.cpp \
    qcontactfilter.cpp \
    qcontactid.cpp \
    qcontactmanager_p.cpp \
    qcontactmanager.cpp \
    qcontactmanagerengine.cpp \
    qcontactmanagerenginefactory.cpp \
    qcontactmanagerenginev2wrapper_p.cpp \
    qcontactobserver.cpp \
    qcontactrelationship.cpp \
    qcontactsortorder.cpp

contains(QT_CONFIG, jsondb) {
    isEmpty(CONTACTS_DEFAULT_ENGINE): CONTACTS_DEFAULT_ENGINE=jsondb
}

maemo6|meego {
    isEmpty(CONTACTS_DEFAULT_ENGINE): CONTACTS_DEFAULT_ENGINE=tracker
}

wince* {
    isEmpty(CONTACTS_DEFAULT_ENGINE): CONTACTS_DEFAULT_ENGINE=wince
}

symbian {
    isEmpty(CONTACTS_DEFAULT_ENGINE): CONTACTS_DEFAULT_ENGINE=symbian

    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = ALL -TCB
    TARGET.UID3 = 0x2002AC7A

    LIBS += -lefsrv

    CONTACTS_DEPLOYMENT.sources = QtContacts.dll
    CONTACTS_DEPLOYMENT.path = /sys/bin
    DEPLOYMENT += CONTACTS_DEPLOYMENT
}

simulator {
    isEmpty(CONTACTS_DEFAULT_ENGINE): CONTACTS_DEFAULT_ENGINE=simulator

    SOURCES += contactconnection_simulator.cpp engines/qcontactmemorybackenddata_simulator.cpp
    PRIVATE_HEADERS += contactconnection_simulator_p.h engines/qcontactmemorybackenddata_simulator_p.h
}

!isEmpty(CONTACTS_DEFAULT_ENGINE): DEFINES += Q_CONTACTS_DEFAULT_ENGINE=$$CONTACTS_DEFAULT_ENGINE

HEADERS += qtaddoncontactsversion.h $$PUBLIC_HEADERS $$PRIVATE_HEADERS
