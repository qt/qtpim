include(../auto.pri)

QT += contacts

SOURCES  += tst_qcontactrelationship.cpp

# App local deployment
#wince* {
#    QCONTACTACTION_PLUGINS_DEPLOY.sources = $$OUTPUT_DIR/build/tests/bin/plugins/contacts/$$mobilityDeployFilename(contacts_sendemailaction).dll

#    QCONTACTACTION_PLUGINS_DEPLOY.path = ./plugins/contacts

#    DEPLOYMENT += QCONTACTACTION_PLUGINS_DEPLOY
#}
