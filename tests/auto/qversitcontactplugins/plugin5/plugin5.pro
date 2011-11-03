TEMPLATE = lib
CONFIG += plugin testplugin
TARGET = $$qtLibraryTarget(versit_plugin5)
PLUGIN_TYPE=versit
DESTDIR = $$QT.versit.plugins/versit
QT += contacts versit
SOURCES += plugin5.cpp
HEADERS += plugin5.h

# Enable installation of plugin in "make install"
target.path += $$[QT_INSTALL_PLUGINS]/versit
INSTALLS += target
