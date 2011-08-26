TEMPLATE = lib
CONFIG += qt debug warn_on depend_includepath plugin
DESTDIR = $$QT.contacts.plugins/contacts

qtAddLibrary(QtTest)

QMAKE_LIBS += -Wl,-rpath,$${QT.contacts.libs}

QT += contacts

INCLUDEPATH += ../../

SOURCES  += tst_qcontactjsondbasync.cpp
HEADERS += ../../qcontactmanagerdataholder.h

QCONTACTASYNC_PLUGINS_DEPLOY.path = ./plugins/contacts

DEPLOYMENT += QCONTACTASYNC_PLUGINS_DEPLOY

