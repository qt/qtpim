load(qt_module)

TARGET = QtContacts
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
    qcontactengineid.h \

PRIVATE_HEADERS += \
    qcontact_p.h \
    qcontactabstractrequest_p.h \
    qcontactactiondescriptor_p.h \
    qcontactactionmanager_p.h \
    qcontactactiontarget_p.h \
    qcontactchangeset_p.h \
    qcontactdetail_p.h \
    qcontactfetchhint_p.h \
    qcontactfilter_p.h \
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
    qcontactsortorder.cpp \
    qcontactengineid.cpp \

contains(QT_CONFIG, jsondb) {
    isEmpty(CONTACTS_DEFAULT_ENGINE): CONTACTS_DEFAULT_ENGINE=jsondb
}

!isEmpty(CONTACTS_DEFAULT_ENGINE): DEFINES += Q_CONTACTS_DEFAULT_ENGINE=$$CONTACTS_DEFAULT_ENGINE

HEADERS += qtcontactsversion.h $$PUBLIC_HEADERS $$PRIVATE_HEADERS
