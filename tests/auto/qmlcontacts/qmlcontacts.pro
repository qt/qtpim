TEMPLATE=app
TARGET=tst_qmlcontacts
SOURCES += tst_qmlcontacts.cpp
OTHER_FILES += testcases/tst_contacts.qml \
               testcases/tst_contactdetail.qml \
               testcases/tst_contact_signals.qml \
               testcases/contactsTestHelper.qml \
               testcases/tst_contact_extended_detail_e2e.qml
QT += qmltest contacts versit
