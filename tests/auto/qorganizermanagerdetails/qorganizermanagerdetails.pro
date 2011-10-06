include(../auto.pri)

QT += organizer

SOURCES  += tst_qorganizermanagerdetails.cpp
HEADERS += ../qorganizermanagerdataholder.h

#symbian: {
#    TARGET.CAPABILITY = ReadUserData \
#                        WriteUserData
#}

#maemo*:CONFIG += insignificant_test
