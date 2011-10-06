include(../auto.pri)

QT += contacts versit versit-private

HEADERS += tst_qversitcontactexporter.h
SOURCES += tst_qversitcontactexporter.cpp

#symbian: {
#    TARGET.CAPABILITY = ALL \
#        -TCB
#    LIBS += -lws32 \
#        -lbafl
#}
