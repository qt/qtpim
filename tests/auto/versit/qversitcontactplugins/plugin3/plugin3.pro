TEMPLATE = lib
CONFIG += plugin testplugin
TARGET = $$qtLibraryTarget(versit_plugin3)
PLUGIN_TYPE=versit
DESTDIR = $$QT.versit.plugins/versit
QT += contacts versit
SOURCES += plugin3.cpp
HEADERS += plugin3.h

# Enable installation of plugin in "make install"
target.path += $$[QT_INSTALL_PLUGINS]/versit
INSTALLS += target

OTHER_FILES += plugin3.json
