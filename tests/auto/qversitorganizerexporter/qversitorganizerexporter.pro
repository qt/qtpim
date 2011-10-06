include(../auto.pri)

QT += organizer versit versitorganizer

HEADERS += tst_qversitorganizerexporter.h
SOURCES += tst_qversitorganizerexporter.cpp

#symbian: {
#    TARGET.CAPABILITY = ALL \
#        -TCB
#    LIBS += -lws32 \
#        -lbafl
#}
