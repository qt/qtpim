include(../auto.pri)

QMAKE_LIBS += -Wl,-rpath,$${QT.organizer.libs}

QT += organizer

SOURCES  += tst_qorganizermanagerdetails.cpp
HEADERS += ../qorganizermanagerdataholder.h

#symbian: {
#    TARGET.CAPABILITY = ReadUserData \
#                        WriteUserData
#}

#maemo*:CONFIG += insignificant_test
