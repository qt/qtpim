TEMPLATE = lib
CONFIG += plugin testplugin
TARGET = $$qtLibraryTarget(versit_plugin4)
PLUGIN_TYPE=versit
DESTDIR = $$QT.versit.plugins/versit
QT += contacts versit
SOURCES += plugin4.cpp
HEADERS += plugin4.h
