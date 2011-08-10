include(../auto.pri)

QMAKE_LIBS += -Wl,-rpath,$${QT.contacts.libs}

QT += organizer versit versitorganizer

HEADERS += tst_qversitorganizerexporter.h
SOURCES += tst_qversitorganizerexporter.cpp

#symbian: {
#    TARGET.CAPABILITY = ALL \
#        -TCB
#    LIBS += -lws32 \
#        -lbafl
#}
