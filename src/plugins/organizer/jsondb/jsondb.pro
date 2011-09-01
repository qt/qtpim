load(qt_module)

QT += organizer organizer-private network jsondb jsondb-private

TARGET = qtorganizer_jsondb
target.path += $$[QT_INSTALL_PLUGINS]/organizer
INSTALLS += target

load(qt_plugin)

DESTDIR = $$QT.organizer.plugins/organizer

HEADERS += \
    qorganizerjsondbengine.h \
    qorganizerjsondbrequestthread.h \
    qorganizerjsondbenginefactory.h \
    qorganizerjsondbid.h \
    qorganizerjsondbrequestmanager.h \
    qorganizerjsondbstring_p.h

SOURCES += \
    qorganizerjsondbengine.cpp \
    qorganizerjsondbrequestthread.cpp \
    qorganizerjsondbenginefactory.cpp \
    qorganizerjsondbid.cpp \
    qorganizerjsondbrequestmanager.cpp \
    qorganizerjsondbstring.cpp
