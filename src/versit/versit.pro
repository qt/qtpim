TARGET = QtVersit
QT = core contacts

MODULE_PLUGIN_TYPES = \
    versit

QMAKE_DOCS = $$PWD/doc/qtversit.qdocconf

PUBLIC_HEADERS += \
    qversitglobal.h \
    qversitdocument.h \
    qversitproperty.h \
    qversitreader.h \
    qversitwriter.h \
    qversitcontactexporter.h \
    qversitcontactimporter.h \
    qversitcontacthandler.h \
    qversitresourcehandler.h

PRIVATE_HEADERS += \
    qversitdocument_p.h \
    qversitdocumentwriter_p.h \
    qversitproperty_p.h \
    qversitreader_p.h \
    qversitwriter_p.h \
    qvcard21writer_p.h \
    qvcard30writer_p.h \
    qvcardrestorehandler_p.h \
    qversitcontactexporter_p.h \
    qversitcontactimporter_p.h \
    qversitdefs_p.h \
    qversitcontactsdefs_p.h \
    qversitcontactpluginloader_p.h \
    qversitutils_p.h \
    qversitpluginsearch_p.h

SOURCES += \
    qversitdocument.cpp \
    qversitdocument_p.cpp \
    qversitdocumentwriter_p.cpp \
    qversitproperty.cpp \
    qversitreader.cpp \
    qversitreader_p.cpp \
    qversitwriter.cpp \
    qversitwriter_p.cpp \
    qvcard21writer.cpp \
    qvcard30writer.cpp \
    qvcardrestorehandler_p.cpp \
    qversitcontactexporter.cpp \
    qversitcontactexporter_p.cpp \
    qversitcontactimporter.cpp \
    qversitcontactimporter_p.cpp \
    qversitresourcehandler.cpp \
    qversitcontacthandler.cpp \
    qversitcontactpluginloader_p.cpp \
    qversitutils.cpp

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

load(qt_module)
