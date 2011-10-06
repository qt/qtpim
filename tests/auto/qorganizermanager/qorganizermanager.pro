include(../auto.pri)

QT += organizer organizer-private

SOURCES  += tst_qorganizermanager.cpp
HEADERS += ../qorganizermanagerdataholder.h

#symbian: {
#    TARGET.CAPABILITY = ReadUserData \
#                        WriteUserData
#}

#maemo*:CONFIG += insignificant_test
