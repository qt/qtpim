include(../auto.pri)

QMAKE_LIBS += -Wl,-rpath,$${QT.contacts.libs}

QT += contacts

SOURCES  += tst_qcontactrelationship.cpp

# App local deployment
#symbian|wince* {
#    symbian:QCONTACTACTION_PLUGINS_DEPLOY.sources = contacts_sendemailaction.dll
#    wince*:QCONTACTACTION_PLUGINS_DEPLOY.sources = $$OUTPUT_DIR/build/tests/bin/plugins/contacts/$$mobilityDeployFilename(contacts_sendemailaction).dll

#    QCONTACTACTION_PLUGINS_DEPLOY.path = ./plugins/contacts

#    DEPLOYMENT += QCONTACTACTION_PLUGINS_DEPLOY
#}

#symbian: {
#    TARGET.CAPABILITY = ReadUserData \
#                        WriteUserData
#}
