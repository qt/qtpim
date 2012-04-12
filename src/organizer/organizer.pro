load(qt_module)

TARGET = QtOrganizer
QPRO_PWD = $PWD

CONFIG += module

QT = core-private

DEFINES += QT_BUILD_ORGANIZER_LIB QT_MAKEDLL

load(qt_module_config)

include(details/details.pri)
include(items/items.pri)
include(requests/requests.pri)
include(filters/filters.pri)

PUBLIC_HEADERS += \
    qorganizercollection.h \
    qorganizercollectionchangeset.h \
    qorganizercollectionengineid.h \
    qorganizercollectionid.h \
    qorganizerabstractrequest.h \
    qorganizeritemchangeset.h \
    qorganizeritemdetail.h \
    qorganizeritemfetchhint.h \
    qorganizeritemfilter.h \
    qorganizeritem.h \
    qorganizeritemid.h \
    qorganizeritemengineid.h \
    qorganizeritemobserver.h \
    qorganizermanager.h \
    qorganizermanagerengine.h \
    qorganizermanagerenginefactory.h \
    qorganizerrecurrencerule.h \
    qorganizeritemsortorder.h \
    qorganizerglobal.h \
    qorganizer.h

PRIVATE_HEADERS += \
    qorganizercollection_p.h \
    qorganizercollectionchangeset_p.h \
    qorganizerabstractrequest_p.h \
    qorganizeritemchangeset_p.h \
    qorganizeritem_p.h \
    qorganizeritemdetail_p.h \
    qorganizeritemfilter_p.h \
    qorganizeritemfetchhint_p.h \
    qorganizermanager_p.h \
    qorganizerrecurrencerule_p.h \
    qorganizeritemsortorder_p.h

SOURCES += \
    qorganizercollection.cpp \
    qorganizercollectionchangeset.cpp \
    qorganizercollectionengineid.cpp \
    qorganizercollectionid.cpp \
    qorganizerabstractrequest.cpp \
    qorganizeritemchangeset.cpp \
    qorganizeritem.cpp \
    qorganizeritemdetail.cpp \
    qorganizeritemfetchhint.cpp \
    qorganizeritemfilter.cpp \
    qorganizeritemid.cpp \
    qorganizeritemengineid.cpp \
    qorganizeritemobserver.cpp \
    qorganizermanager.cpp \
    qorganizermanagerengine.cpp \
    qorganizermanagerenginefactory.cpp \
    qorganizerrecurrencerule.cpp \
    qorganizeritemsortorder.cpp \
    qorganizermanager_p.cpp

contains(QT_CONFIG, jsondb) {
    isEmpty(ORGANIZER_DEFAULT_ENGINE): ORGANIZER_DEFAULT_ENGINE=jsondb
}

simulator {
    isEmpty(ORGANIZER_DEFAULT_ENGINE): ORGANIZER_DEFAULT_ENGINE=simulator
}

!isEmpty(ORGANIZER_DEFAULT_ENGINE): DEFINES += Q_ORGANIZER_DEFAULT_ENGINE=$$ORGANIZER_DEFAULT_ENGINE

HEADERS += qtorganizerversion.h $$PUBLIC_HEADERS $$PRIVATE_HEADERS
