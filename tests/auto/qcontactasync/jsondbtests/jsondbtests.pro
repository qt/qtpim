include(../../auto.pri)

QT += contacts

SOURCES  += tst_qcontactjsondbasync.cpp
HEADERS += ../../qcontactmanagerdataholder.h



#TEMPLATE = lib
#CONFIG += qt debug warn_on depend_includepath plugin
#DESTDIR = $$QT.contacts.plugins/contacts

#qtAddLibrary(QtTest)

#QMAKE_LIBS += -Wl,-rpath,$${QT.contacts.libs}

#T += contacts

#INCLUDEPATH += ../../


#QCONTACTASYNC_PLUGINS_DEPLOY.path = ./plugins/contacts

#DEPLOYMENT += QCONTACTASYNC_PLUGINS_DEPLOY

