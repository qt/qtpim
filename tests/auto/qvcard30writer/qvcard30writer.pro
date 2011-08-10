include(../auto.pri)

QMAKE_LIBS += -Wl,-rpath,$${QT.versit.libs}

QT += versit versit-private

DEFINES += QT_ASCII_CAST_WARNINGS

HEADERS += tst_qvcard30writer.h
SOURCES += tst_qvcard30writer.cpp

#symbian: {
#    TARGET.CAPABILITY = ALL \
#        -TCB
#    LIBS += -lws32 \
#        -lbafl
#}
