include(../auto.pri)

QT += versit

HEADERS += tst_qversitdocument.h
SOURCES += tst_qversitdocument.cpp

#symbian: {
#    TARGET.CAPABILITY = ALL \
#        -TCB
#    LIBS += -lws32 \
#        -lbafl
#}
