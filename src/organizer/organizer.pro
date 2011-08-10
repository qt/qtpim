load(qt_module)

TARGET = QtAddOnOrganizer
QPRO_PWD = $PWD

CONFIG += module
MODULE_PRI = ../../modules/qt_organizer.pri

QT = core gui

DEFINES += QT_BUILD_ORGANIZER_LIB QT_MAKEDLL

load(qt_module_config)

include(details/details.pri)
include(items/items.pri)
include(requests/requests.pri)
include(filters/filters.pri)
include(engines/engines.pri)

PUBLIC_HEADERS += \
    qorganizercollection.h \
    qorganizercollectionchangeset.h \
    qorganizercollectionengineid.h \
    qorganizercollectionid.h \
    qorganizerabstractrequest.h \
    qorganizeritemchangeset.h \
    qorganizeritemdetail.h \
    qorganizeritemdetaildefinition.h \
    qorganizeritemdetailfielddefinition.h \
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
    qtorganizerglobal.h \
    qtorganizer.h

PRIVATE_HEADERS += \
    qpim_p.h \
    qlatin1constant_p.h \
    qmalgorithms_p.h \
    qmobilitypluginsearch_p.h \
    qorganizercollection_p.h \
    qorganizercollectionchangeset_p.h \
    qorganizerabstractrequest_p.h \
    qorganizeritemchangeset_p.h \
    qorganizeritem_p.h \
    qorganizeritemdetail_p.h \
    qorganizeritemfilter_p.h \
    qorganizeritemfetchhint_p.h \
    qorganizeritemdetailfielddefinition_p.h \
    qorganizeritemdetaildefinition_p.h \
    qorganizermanager_p.h \
    qorganizermanagerenginev2wrapper_p.h \
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
    qorganizeritemdetaildefinition.cpp \
    qorganizeritemdetailfielddefinition.cpp \
    qorganizeritemfetchhint.cpp \
    qorganizeritemfilter.cpp \
    qorganizeritemid.cpp \
    qorganizeritemengineid.cpp \
    qorganizeritemobserver.cpp \
    qorganizermanager.cpp \
    qorganizermanagerengine.cpp \
    qorganizermanagerenginefactory.cpp \
    qorganizermanagerenginev2wrapper_p.cpp \
    qorganizerrecurrencerule.cpp \
    qorganizeritemsortorder.cpp \
    qorganizermanager_p.cpp

maemo6 {
    isEmpty(ORGANIZER_DEFAULT_ENGINE): ORGANIZER_DEFAULT_ENGINE=mkcal
}

symbian {
    isEmpty(ORGANIZER_DEFAULT_ENGINE): ORGANIZER_DEFAULT_ENGINE=symbian

    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = ALL -TCB

    TARGET.UID3 = 0x2002BFCD

    LIBS += -lefsrv

    # ## Organizer
    # Main library
    ORGANIZER_DEPLOYMENT.sources = QtOrganizer.dll
    ORGANIZER_DEPLOYMENT.path = /sys/bin
    DEPLOYMENT += ORGANIZER_DEPLOYMENT
}

simulator {
    isEmpty(ORGANIZER_DEFAULT_ENGINE): ORGANIZER_DEFAULT_ENGINE=simulator
}

!isEmpty(ORGANIZER_DEFAULT_ENGINE): DEFINES += Q_ORGANIZER_DEFAULT_ENGINE=$$ORGANIZER_DEFAULT_ENGINE

HEADERS += qtorganizerversion.h $$PUBLIC_HEADERS $$PRIVATE_HEADERS