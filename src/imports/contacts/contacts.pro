TARGET  = declarative_contacts
TARGETPATH = QtAddOn/contacts

include(qcontactsimport.pri)
target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
DESTDIR = $$QT.contacts.imports/$$TARGETPATH
INSTALLS += target

qmldir.files += $$PWD/qmldir
qmldir.path +=  $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
INSTALLS += qmldir

QT += declarative contacts versit

# Input
include(details/details.pri)
include(filters/filters.pri)

HEADERS += qdeclarativecontactmodel_p.h \
           qdeclarativecontact_p.h \
           qdeclarativecontactdetail_p.h \
           qdeclarativecontactfilter_p.h \
           qdeclarativecontactmetaobject_p.h \
           qdeclarativecontactimageprovider_p.h \
           qdeclarativecontactsortorder_p.h \
           qdeclarativecontactfetchhint_p.h \
           qdeclarativecontactrelationship_p.h \
           qdeclarativecontactrelationshipmodel_p.h \
           qdeclarativeopenmetaobject_p.h # \
#           qmetaobjectbuilder_p.h \
#           qdeclarativeglobal_p.h


SOURCES += plugin.cpp \
    qdeclarativecontactmodel.cpp \
    qdeclarativecontact.cpp \
    qdeclarativecontactdetail.cpp \
    qdeclarativecontactfilter.cpp \
    qdeclarativecontactmetaobject.cpp \
    qdeclarativecontactimageprovider.cpp \
    qdeclarativecontactsortorder.cpp \
    qdeclarativecontactfetchhint.cpp \
    qdeclarativecontactrelationship.cpp \
    qdeclarativecontactrelationshipmodel.cpp \
    qdeclarativeopenmetaobject.cpp

RESOURCES += contacts.qrc
