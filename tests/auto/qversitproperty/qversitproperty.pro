include(../auto.pri)

QT += versit versit-private

HEADERS += tst_qversitproperty.h
SOURCES += tst_qversitproperty.cpp

#symbian: {
#    TARGET.CAPABILITY = ALL \
#        -TCB
#    LIBS += -lws32 \
#        -lbafl
#}
