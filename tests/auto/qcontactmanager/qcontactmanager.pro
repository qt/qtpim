include(../auto.pri)

QMAKE_LIBS += -Wl,-rpath,$${QT.contacts.libs}

QT += contacts versit

SOURCES  += tst_qcontactmanager.cpp
HEADERS += ../qcontactmanagerdataholder.h

#symbian: {
#    TARGET.CAPABILITY = ReadUserData \
#                        WriteUserData
#
#    contains(S60_VERSION,3.1) {
#        DEFINES += SYMBIAN_BACKEND_S60_VERSION_31
#        LIBS += -lcntmodel
#    }
#    contains(S60_VERSION,3.2) {
#        DEFINES += SYMBIAN_BACKEND_S60_VERSION_32
#        LIBS += -lcntmodel
#    }
#    contains(S60_VERSION,5.0) {
#        DEFINES += SYMBIAN_BACKEND_S60_VERSION_50
#        LIBS += -lcntmodel
#    }
#}

#maemo*:CONFIG += insignificant_test