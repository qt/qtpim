include(../auto.pri)

QT += contacts

SOURCES += tst_qcontact.cpp

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
