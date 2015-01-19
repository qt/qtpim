TARGET = QtContacts
QT = core-private

MODULE_PLUGIN_TYPES = \
    contacts

QMAKE_DOCS = $$PWD/doc/qtcontacts.qdocconf

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
    qcontactcollection.h \
    qcontactcollectionchangeset.h \
    qcontactcollectionid.h \
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
    qcontacts.h

PRIVATE_HEADERS += \
    qcontact_p.h \
    qcontactabstractrequest_p.h \
    qcontactactiondescriptor_p.h \
    qcontactactionmanager_p.h \
    qcontactactiontarget_p.h \
    qcontactchangeset_p.h \
    qcontactcollection_p.h \
    qcontactcollectionchangeset_p.h \
    qcontactdetail_p.h \
    qcontactfetchhint_p.h \
    qcontactfilter_p.h \
    qcontactmanager_p.h \
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
    qcontactcollection.cpp \
    qcontactcollectionchangeset.cpp \
    qcontactcollectionid.cpp \
    qcontactdetail.cpp \
    qcontactfetchhint.cpp \
    qcontactfilter.cpp \
    qcontactid.cpp \
    qcontactmanager_p.cpp \
    qcontactmanager.cpp \
    qcontactmanagerengine.cpp \
    qcontactmanagerenginefactory.cpp \
    qcontactobserver.cpp \
    qcontactrelationship.cpp \
    qcontactsortorder.cpp

!isEmpty(CONTACTS_DEFAULT_ENGINE): DEFINES += Q_CONTACTS_DEFAULT_ENGINE=$$CONTACTS_DEFAULT_ENGINE

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

load(qt_module)
