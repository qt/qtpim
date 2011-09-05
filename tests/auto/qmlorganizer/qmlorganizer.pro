TEMPLATE=app
QT += qmltest versit organizer
TARGET=tst_qmlorganizer
SOURCES += tst_qmlorganizer.cpp
OTHER_FILES += testcases/addEvent.qml \
    testcases/tst_detailfilter.qml \
    testcases/tst_organizeritems.qml \
    testcases/tst_organizermodel.qml \
    testcases/tst_collection.qml
include (../qmlauto.pri)
