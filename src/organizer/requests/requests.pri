INCLUDEPATH += requests \
    ./

PUBLIC_HEADERS += requests/qorganizeritemrequests.h \
    requests/qorganizercollectionfetchrequest.h \
    requests/qorganizercollectionremoverequest.h \
    requests/qorganizercollectionsaverequest.h \
    requests/qorganizeritemfetchrequest.h \
    requests/qorganizeritemfetchforexportrequest.h \
    requests/qorganizeritemfetchbyidrequest.h \
    requests/qorganizeritemoccurrencefetchrequest.h \
    requests/qorganizeritemidfetchrequest.h \
    requests/qorganizeritemremoverequest.h \
    requests/qorganizeritemsaverequest.h \
    requests/qorganizeritemremovebyidrequest.h

PRIVATE_HEADERS += requests/qorganizeritemrequests_p.h

SOURCES += \
    requests/qorganizercollectionfetchrequest.cpp \
    requests/qorganizercollectionremoverequest.cpp \
    requests/qorganizercollectionsaverequest.cpp \
    requests/qorganizeritemfetchrequest.cpp \
    requests/qorganizeritemfetchforexportrequest.cpp \
    requests/qorganizeritemfetchbyidrequest.cpp \
    requests/qorganizeritemoccurrencefetchrequest.cpp \
    requests/qorganizeritemidfetchrequest.cpp \
    requests/qorganizeritemremoverequest.cpp \
    requests/qorganizeritemsaverequest.cpp \
    requests/qorganizeritemremovebyidrequest.cpp
