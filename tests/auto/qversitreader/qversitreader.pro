include(../auto.pri)

QMAKE_LIBS += -Wl,-rpath,$${QT.versit.libs}

QT += versit versit-private

HEADERS += tst_qversitreader.h
SOURCES += tst_qversitreader.cpp

#symbian: {
#    TARGET.CAPABILITY = ALL \
#        -TCB
#    LIBS += -lws32 \
#        -lbafl
#}
