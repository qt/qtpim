include(../auto.pri)

QMAKE_LIBS += -Wl,-rpath,$${QT.contacts.libs}

QT += organizer versit versitorganizer

HEADERS += tst_qversitorganizerimporter.h
SOURCES += tst_qversitorganizerimporter.cpp

#symbian: {
#    TARGET.CAPABILITY = ALL \
#        -TCB
#    LIBS += -lws32 \
#        -lbafl
#}
