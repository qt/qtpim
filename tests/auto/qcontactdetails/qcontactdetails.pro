include(../auto.pri)

QMAKE_LIBS += -Wl,-rpath,$${QT.contacts.libs}

QT += contacts

SOURCES  += tst_qcontactdetails.cpp

#symbian: {
#    TARGET.CAPABILITY = ReadUserData \
#                        WriteUserData
#}
