include(../../auto.pri)

TEMPLATE=subdirs
SUBDIRS += unittest

# Apart from the fact that the service framework is not working in Qt 5.x,
# the tests do not compile either (even if the check below passes)
#qtHaveModule(serviceframework) {
#    DEFINES += INCLUDE_TESTACTIONS
#    SUBDIRS += testactions
#}
