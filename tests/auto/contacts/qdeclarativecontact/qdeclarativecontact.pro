include(../../auto.pri)

QT += contacts qml versit

DEFINES+=IGNORE_METAOBJECTBUILDER_EXPORT

HEADERS += ../../../src/import/contacts/qdeclarativecontactdetail_p.h\
           ../../../src/import/contacts/qdeclarativecontactrelationship_p.h\
           ../../../src/import/contacts/qdeclarativecontactfetchhint_p.h\
           ../../../src/import/contacts/qdeclarativecontactmodel_p.h\
           ../../../src/import/contacts/qdeclarativecontactsortorder_p.h\
           ../../../src/import/contacts/qdeclarativecontactfilter_p.h\
           ../../../src/import/contacts/qdeclarativecontact_p.h\
           ../../../src/import/contacts/qdeclarativecontactimageprovider_p.h\
           ../../../src/import/contacts/qdeclarativecontactrelationshipmodel_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactaddress_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactname_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactanniversary_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactnickname_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactavatar_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactnote_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactbirthday_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactonlineaccount_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactdetails_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactorganization_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactdisplaylabel_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactphonenumber_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactemailaddress_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactpresence_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactfamily_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactringtone_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactfavorite_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactsynctarget_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactgender_p.h\
           ../../../src/import/contacts/details/qdeclarativecontacttag_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactgeolocation_p.h\
           ../../../src/import/contacts/details/qdeclarativecontacttimestamp_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactglobalpresence_p.h\
           ../../../src/import/contacts/details/qdeclarativecontactguid_p.h\
           ../../../src/import/contacts/details/qdeclarativecontacturl_p.h \
           ../../../src/import/contacts/details/qdeclarativecontacthobby_p.h \
           ../../../src/import/contacts/filters/qdeclarativecontactactionfilter_p.h\
           ../../../src/import/contacts/filters/qdeclarativecontactintersectionfilter_p.h \
           ../../../src/import/contacts/filters/qdeclarativecontactchangelogfilter_p.h  \
           ../../../src/import/contacts/filters/qdeclarativecontactinvalidfilter_p.h \
           ../../../src/import/contacts/filters/qdeclarativecontactdetailfilter_p.h     \
           ../../../src/import/contacts/filters/qdeclarativecontactidfilter_p.h \
           ../../../src/import/contacts/filters/qdeclarativecontactdetailrangefilter_p.h \
           ../../../src/import/contacts/filters/qdeclarativecontactrelationshipfilter_p.h \
           ../../../src/import/contacts/filters/qdeclarativecontactfilters_p.h   \
           ../../../src/import/contacts/filters/qdeclarativecontactunionfilter_p.h

SOURCES += tst_qdeclarativecontact.cpp \
           ../../../src/import/contacts/qdeclarativecontactdetail.cpp\
           ../../../src/import/contacts/qdeclarativecontactmetaobject.cpp\
           ../../../src/import/contacts/qdeclarativecontactrelationship.cpp\
           ../../../src/import/contacts/qdeclarativecontactfetchhint.cpp\
           ../../../src/import/contacts/qdeclarativecontactmodel.cpp\
           ../../../src/import/contacts/qdeclarativecontactsortorder.cpp\
           ../../../src/import/contacts/qdeclarativecontactfilter.cpp\
           ../../../src/import/contacts/qdeclarativecontact.cpp\
           ../../../src/import/contacts/qdeclarativecontactrelationshipmodel.cpp

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
