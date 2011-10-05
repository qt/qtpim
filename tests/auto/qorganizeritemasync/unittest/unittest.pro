include(../../auto.pri)

TARGET = tst_qorganizeritemasync
QMAKE_LIBS += -Wl,-rpath,$${QT.organizer.libs}

QT += organizer

SOURCES  += tst_qorganizeritemasync.cpp
HEADERS += ../../qorganizermanagerdataholder.h

#symbian: {
#    TARGET.CAPABILITY = ReadUserData \
#                        WriteUserData
#}

#maemo*:CONFIG += insignificant_test
