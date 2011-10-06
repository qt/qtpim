include(../auto.pri)

QT += versit versit-private

HEADERS += tst_qversitwriter.h
SOURCES += tst_qversitwriter.cpp

#symbian: {
#    TARGET.CAPABILITY = ALL \
#        -TCB
#    LIBS += -lws32 \
#        -lbafl
#}
