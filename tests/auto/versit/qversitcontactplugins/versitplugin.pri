NAME = $$TARGET

TEMPLATE = lib
CONFIG += plugin testplugin
TARGET = $$qtLibraryTarget(versit_$$NAME)
PLUGIN_TYPE = versit
QT += contacts versit

debug_and_release {
    CONFIG(debug, debug|release): \
        INFIX = debug/
    else: \
        INFIX = release/
}
DESTDIR = $$shadowed($$PWD)/unittest/$${INFIX}plugins/versit

SOURCES += $${NAME}.cpp
HEADERS += $${NAME}.h

OTHER_FILES += $${NAME}.json
