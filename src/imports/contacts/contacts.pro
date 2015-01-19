QT += qml contacts versit

include(details/details.pri)
include(filters/filters.pri)

HEADERS += qdeclarativecontactmodel_p.h \
           qdeclarativecontact_p.h \
           qdeclarativecontactcollection_p.h \
           qdeclarativecontactdetail_p.h \
           qdeclarativecontactfilter_p.h \
           qdeclarativecontactsortorder_p.h \
           qdeclarativecontactfetchhint_p.h \
           qdeclarativecontactrelationship_p.h \
           qdeclarativecontactrelationshipmodel_p.h \

SOURCES += plugin.cpp \
    qdeclarativecontactmodel.cpp \
    qdeclarativecontact.cpp \
    qdeclarativecontactcollection.cpp \
    qdeclarativecontactdetail.cpp \
    qdeclarativecontactfilter.cpp \
    qdeclarativecontactsortorder.cpp \
    qdeclarativecontactfetchhint.cpp \
    qdeclarativecontactrelationship.cpp \
    qdeclarativecontactrelationshipmodel.cpp \

RESOURCES += contacts.qrc

OTHER_FILES += contacts.json

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0

load(qml_plugin)
