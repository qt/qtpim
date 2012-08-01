include(../../auto.pri)

QT += contacts organizer versit versitorganizer versit-private

DEFINES += TESTDATA_DIR=\\\"$$PWD/\\\"

HEADERS += tst_qversit.h
SOURCES += tst_qversit.cpp
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
