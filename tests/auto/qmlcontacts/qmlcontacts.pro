TEMPLATE=app
TARGET=tst_qmlcontacts
SOURCES += tst_qmlcontacts.cpp
OTHER_FILES += testcases/tst_contacts.qml \
               testcases/contactsTestHelper.qml
QT += qmltest contacts versit
