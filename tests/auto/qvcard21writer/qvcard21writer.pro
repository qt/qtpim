include(../auto.pri)

QT += versit versit-private

HEADERS += tst_qvcard21writer.h
SOURCES += tst_qvcard21writer.cpp

#symbian: {
#    TARGET.CAPABILITY = ALL \
#        -TCB
#    LIBS += -lws32 \
#        -lbafl
#}
