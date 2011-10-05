include(../auto.pri)

QMAKE_LIBS += -Wl,-rpath,$${QT.organizer.libs}

QT += organizer organizer-private

SOURCES  += tst_qorganizere2e.cpp
