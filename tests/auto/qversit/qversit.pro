include(../auto.pri)

QT += contacts organizer versit versitorganizer versit-private

wince* {
    DEFINES+= TESTDATA_DIR=\\\"./\\\"
}else {
    DEFINES += TESTDATA_DIR=\\\"$$PWD/\\\"
}

HEADERS += tst_qversit.h
SOURCES += tst_qversit.cpp
