include(../../auto.pri)

QMAKE_LIBS += -Wl,-rpath,$${QT.contacts.libs}

QT += contacts

SOURCES  += tst_qcontactasync.cpp
HEADERS += ../../qcontactmanagerdataholder.h

# App local deployment
#symbian:QCONTACTASYNC_PLUGINS_DEPLOY.sources = contacts_maliciousplugin.dll
#wince*: QCONTACTASYNC_PLUGINS_DEPLOY.sources = $$OUTPUT_DIR/build/tests/bin/plugins/contacts/$$mobilityDeployFilename(contacts_maliciousplugin).dll
#maemo*: QCONTACTASYNC_PLUGINS_DEPLOY.sources = $$OUTPUT_DIR/build/tests/bin/plugins/contacts/lib$$mobilityDeployFilename(contacts_maliciousplugin).so

#QCONTACTASYNC_PLUGINS_DEPLOY.path = ./plugins/contacts

#DEPLOYMENT += QCONTACTASYNC_PLUGINS_DEPLOY

#symbian: {
#    TARGET.CAPABILITY = ReadUserData \
#                        WriteUserData
#}

#maemo*:CONFIG += insignificant_test
