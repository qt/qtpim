INCLUDEPATH += filters \
    ./

PUBLIC_HEADERS += \
    filters/qorganizeritemfilters.h \
    filters/qorganizeritemdetailfilter.h \
    filters/qorganizeritemdetailfieldfilter.h \
    filters/qorganizeritemdetailrangefilter.h \
    filters/qorganizeritemintersectionfilter.h \
    filters/qorganizeriteminvalidfilter.h \
    filters/qorganizeritemidfilter.h \
    filters/qorganizeritemcollectionfilter.h \
    filters/qorganizeritemunionfilter.h

PRIVATE_HEADERS += \
    filters/qorganizeritemdetailfilter_p.h \
    filters/qorganizeritemdetailfieldfilter_p.h \
    filters/qorganizeritemdetailrangefilter_p.h \
    filters/qorganizeritemintersectionfilter_p.h \
    filters/qorganizeritemidfilter_p.h \
    filters/qorganizeritemcollectionfilter_p.h \
    filters/qorganizeritemunionfilter_p.h

SOURCES += \
    filters/qorganizeritemdetailfilter.cpp \
    filters/qorganizeritemdetailfieldfilter.cpp \
    filters/qorganizeritemdetailrangefilter.cpp \
    filters/qorganizeritemintersectionfilter.cpp \
    filters/qorganizeriteminvalidfilter.cpp \
    filters/qorganizeritemidfilter.cpp \
    filters/qorganizeritemcollectionfilter.cpp \
    filters/qorganizeritemunionfilter.cpp
