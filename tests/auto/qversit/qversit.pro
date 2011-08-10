include(../auto.pri)

QMAKE_LIBS += -Wl,-rpath,$${QT.versit.libs}

QT += contacts organizer versit versitorganizer versit-private

#symbian*: {
#    ICAL_TESTDATA.sources = testdata_ics/*
#    ICAL_TESTDATA.path = testdata_ics
#    DEPLOYMENT += ICAL_TESTDATA
#    VCARD_TESTDATA.sources = testdata_vcf/*
#    VCARD_TESTDATA.path = testdata_vcf
#    DEPLOYMENT += VCARD_TESTDATA
#}

#wince* {
#    DEFINES+= TESTDATA_DIR=\\\"./\\\"
#}else:!symbian {
    DEFINES += TESTDATA_DIR=\\\"$$PWD/\\\"
#}

HEADERS += tst_qversit.h
SOURCES += tst_qversit.cpp

#symbian: {
#    TARGET.CAPABILITY = ALL \
#        -TCB
#    LIBS += -lws32 \
#        -lbafl
#}

#maemo*:CONFIG += insignificant_test
