TEMPLATE=app
QT += qmltest versit organizer
TARGET=tst_qmlorganizer
SOURCES += tst_qmlorganizer.cpp
OTHER_FILES += testcases/addEvent.qml \
    testcases/QOrganizerTestUtility.qml \
    testcases/tst_detailfilter.qml \
    testcases/tst_organizeritem.qml \
    testcases/tst_organizeritems.qml \
    testcases/tst_organizermodel.qml \
    testcases/tst_collection.qml \
    testcases/tst_organizeraudiblereminder.qml \
    testcases/tst_organizervisualreminder.qml \
    testcases/tst_organizeremailreminder.qml \
    testcases/tst_organizercollectionfilter.qml \
    testcases/tst_organizerunionfilter.qml \
    testcases/tst_organizerintersectionfilter.qml \
    testcases/tst_organizercollectionfilter.qml \
    testcases/tst_organizeritemdetail.qml \
    testcases/tst_organizere2e.qml \
    testcases/tst_organizerrecurrence.qml \
    testcases/tst_organizermodelupdate.qml \
    testcases/tst_organizer_versit_export_import_e2e.qml
include (../qmlauto.pri)
