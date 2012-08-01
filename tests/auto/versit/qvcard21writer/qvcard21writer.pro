include(../../auto.pri)

QT += versit versit-private

HEADERS += tst_qvcard21writer.h
SOURCES += tst_qvcard21writer.cpp

CONFIG+=insignificant_test # QTBUG-25382
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
