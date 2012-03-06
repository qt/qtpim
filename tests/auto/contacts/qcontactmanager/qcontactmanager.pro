include(../../auto.pri)

QT += contacts versit contacts-private

SOURCES  += tst_qcontactmanager.cpp
HEADERS += ../qcontactmanagerdataholder.h
HEADERS += ../../jsondbprocess.h

OTHER_FILES += lazy.json
