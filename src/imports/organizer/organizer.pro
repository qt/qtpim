TARGET  = declarative_organizer
TARGETPATH = QtOrganizer

include(qorganizerimport.pri)
target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
DESTDIR = $$QT.organizer.imports/$$TARGETPATH
INSTALLS += target

qmldir.files += $$PWD/qmldir
qmldir.path +=  $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
INSTALLS += qmldir

qmlplugintypes.files += $$PWD/plugins.qmltypes
qmlplugintypes.path +=  $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
INSTALLS += qmlplugintypes

QT += qml organizer versit versitorganizer

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
