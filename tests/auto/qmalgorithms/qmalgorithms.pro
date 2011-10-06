include(../auto.pri)

unix:QMAKE_LIBS += -Wl,-rpath,$${QT.contacts.libs}

QT += organizer-private

SOURCES  += tst_qmalgorithms.cpp
