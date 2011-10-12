TARGET  = declarative_organizer
TARGETPATH = QtAddOn/organizer

include(qorganizerimport.pri)
target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
DESTDIR = $$QT.organizer.imports/$$TARGETPATH
INSTALLS += target

qmldir.files += $$PWD/qmldir
qmldir.path +=  $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
INSTALLS += qmldir

QT += declarative organizer versit versitorganizer

# Input
HEADERS += qdeclarativeorganizermodel_p.h \
           qdeclarativeorganizeritem_p.h \
           qdeclarativeorganizeritemdetail_p.h \
           qdeclarativeorganizeritemfilter_p.h \
           qdeclarativeorganizerrecurrencerule_p.h \
           qdeclarativeorganizercollection_p.h \
           qdeclarativeorganizeritemsortorder_p.h \
           qdeclarativeorganizeritemfetchhint_p.h

SOURCES += plugin.cpp \
           qdeclarativeorganizeritem.cpp \
           qdeclarativeorganizeritemdetail.cpp \
           qdeclarativeorganizermodel.cpp \
           qdeclarativeorganizeritemfilter.cpp \
           qdeclarativeorganizercollection.cpp \
           qdeclarativeorganizeritemsortorder.cpp \
           qdeclarativeorganizerrecurrencerule.cpp \
           qdeclarativeorganizeritemfetchhint.cpp
