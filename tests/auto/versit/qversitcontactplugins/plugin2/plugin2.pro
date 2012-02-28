TEMPLATE = lib
CONFIG += plugin testplugin
TARGET = $$qtLibraryTarget(versit_plugin2)
PLUGIN_TYPE=versit
DESTDIR = $$QT.versit.plugins/versit
QT += contacts versit
SOURCES += plugin2.cpp
HEADERS += plugin2.h

# Enable installation of plugin in "make install"
target.path += $$[QT_INSTALL_PLUGINS]/versit
INSTALLS += target

OTHER_FILES += plugin2.json
