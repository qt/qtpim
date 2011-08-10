include(../auto.pri)

QMAKE_LIBS += -Wl,-rpath,$${QT.versit.libs}

QT += versit versit-private

HEADERS += tst_qvcard21writer.h
SOURCES += tst_qvcard21writer.cpp

#symbian: {
#    TARGET.CAPABILITY = ALL \
#        -TCB
#    LIBS += -lws32 \
#        -lbafl
#}
