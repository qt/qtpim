TEMPLATE=app
TARGET=tst_qmlorganizer
QT += versit organizer
CONFIG += qmltestcase
CONFIG += console
SOURCES += tst_qmlorganizer.cpp

importFiles.files = testcases

importFiles.path = .
DEPLOYMENT += importFiles
