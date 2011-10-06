include(../auto.pri)

QT += versit versit-private

HEADERS += tst_qversitreader.h
SOURCES += tst_qversitreader.cpp

#symbian: {
#    TARGET.CAPABILITY = ALL \
#        -TCB
#    LIBS += -lws32 \
#        -lbafl
#}
