TEMPLATE = app
TARGET = calendardemo
QT += organizer versit widgets

# Input
SOURCES += src/main.cpp \
           src/calendardemo.cpp \
           src/monthpage.cpp \
           src/daypage.cpp \
           src/eventeditpage.cpp \
           src/todoeditpage.cpp \
           src/journaleditpage.cpp \
           src/eventoccurrenceeditpage.cpp \
           src/addcalendarpage.cpp \
           src/editcalendarspage.cpp

HEADERS += src/calendardemo.h \
           src/monthpage.h \
           src/daypage.h \
           src/eventeditpage.h \
           src/todoeditpage.h \
           src/journaleditpage.h \
           src/eventoccurrenceeditpage.h \
           src/addcalendarpage.h \
           src/editcalendarspage.h

target.path  = $$[QT_INSTALL_EXAMPLES]/organizer/calendardemo
INSTALLS    += target
