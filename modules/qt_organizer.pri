QT.organizer.VERSION = 2.0.0
QT.organizer.MAJOR_VERSION = 2
QT.organizer.MINOR_VERSION = 0
QT.organizer.PATCH_VERSION = 0

QT.organizer.name = QtOrganizer
QT.organizer.bins = $$QT_MODULE_BIN_BASE
QT.organizer.includes = $$QT_MODULE_INCLUDE_BASE $$QT_MODULE_INCLUDE_BASE/QtOrganizer
QT.organizer.private_includes = $$QT_MODULE_INCLUDE_BASE/QtOrganizer/$$QT.organizer.VERSION
QT.organizer.sources = $$QT_MODULE_BASE/src/organizer
QT.organizer.libs = $$QT_MODULE_LIB_BASE
QT.organizer.plugins = $$QT_MODULE_PLUGIN_BASE
QT.organizer.imports = $$QT_MODULE_IMPORT_BASE
QT.organizer.depends = core
QT.organizer.DEFINES = QT_ORGANIZER_LIB

QT_CONFIG += organizer
