include(../../auto.pri)

TEMPLATE=subdirs
SUBDIRS += unittest

qtHaveModule(serviceframework) {
    DEFINES += INCLUDE_TESTACTIONS
    SUBDIRS += testactions
}
