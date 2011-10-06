include(../../auto.pri)

TARGET = tst_qorganizeritemasync

QT += organizer

SOURCES  += tst_qorganizeritemasync.cpp
HEADERS += ../../qorganizermanagerdataholder.h

#symbian: {
#    TARGET.CAPABILITY = ReadUserData \
#                        WriteUserData
#}

#maemo*:CONFIG += insignificant_test
