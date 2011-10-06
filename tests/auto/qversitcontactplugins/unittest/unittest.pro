include(../../auto.pri)

TARGET = tst_qversitcontactplugins

QT += contacts versit

SOURCES += tst_qversitcontactplugins.cpp

#symbian: {
#    TARGET.CAPABILITY = ALL \
#        -TCB
#    LIBS += -lws32 \
#        -lbafl
#}
