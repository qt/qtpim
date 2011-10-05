include(../../auto.pri)

QMAKE_LIBS += -Wl,-rpath,$${QT.contacts.libs}

TARGET = tst_qcontactmanagerplugins
QT += widgets contacts

SOURCES += tst_qcontactmanagerplugins.cpp

#TESTDLLS = \
#    $$mobilityDeployFilename(contacts_testdummy) \
#    $$mobilityDeployFilename(contacts_testotherdummy) \
#    $$mobilityDeployFilename(contacts_testdummycopy) \
#    $$mobilityDeployFilename(contacts_testdummyinvalid) \
#    $$mobilityDeployFilename(contacts_testdummyempty) \
#    $$mobilityDeployFilename(contacts_testdummymemory)

# App local deployment
#symbian|wince* {
#    symbian:QCONTACTMANAGER_PLUGINS_DEPLOY.sources = $$join(TESTDLLS, ".dll ", " ", ".dll")
#    wince*:QCONTACTMANAGER_PLUGINS_DEPLOY.sources = $$join(TESTDLLS, ".dll $$OUTPUT_DIR/build/tests/bin/plugins/contacts/", "$$OUTPUT_DIR/build/tests/bin/plugins/contacts/", ".dll")
#    QCONTACTMANAGER_PLUGINS_DEPLOY.path = ./plugins/contacts
#    DEPLOYMENT += QCONTACTMANAGER_PLUGINS_DEPLOY
#}

# all else, we need to copy and install the DLLs to the right place (in a subdir for this test)

#symbian: {
#    TARGET.CAPABILITY = ReadUserData \
#                        WriteUserData
#}
