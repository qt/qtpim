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

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0

load(qml_plugin)
