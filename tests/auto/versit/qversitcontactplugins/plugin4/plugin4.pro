TEMPLATE = lib
CONFIG += plugin testplugin
TARGET = $$qtLibraryTarget(versit_plugin4)
PLUGIN_TYPE=versit
DESTDIR = $$QT.versit.plugins/versit
QT += contacts versit
SOURCES += plugin4.cpp
HEADERS += plugin4.h

# Enable installation of plugin in "make install"
target.path += $$[QT_INSTALL_PLUGINS]/versit
INSTALLS += target

OTHER_FILES += plugin4.json
