include(../auto.pri)

QMAKE_LIBS += -Wl,-rpath,$${QT.organizer.libs}

QT += organizer

SOURCES  += tst_qorganizeritemdetails.cpp

#symbian: {
#    TARGET.CAPABILITY = ReadUserData \
#                        WriteUserData
#}