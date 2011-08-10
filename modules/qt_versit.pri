QT.versit.VERSION = 5.0.0
QT.versit.MAJOR_VERSION = 5
QT.versit.MINOR_VERSION = 0
QT.versit.PATCH_VERSION = 0

QT.versit.name = QtAddOnVersit
QT.versit.bins = $$QT_MODULE_BIN_BASE
QT.versit.includes = $$QT_MODULE_INCLUDE_BASE $$QT_MODULE_INCLUDE_BASE/QtAddOnVersit
QT.versit.private_includes = $$QT_MODULE_INCLUDE_BASE/QtAddOnVersit/$$QT.versit.VERSION
QT.versit.sources = $$QT_MODULE_BASE/src/versit
QT.versit.libs = $$QT_MODULE_LIB_BASE
QT.versit.plugins = $$QT_MODULE_PLUGIN_BASE
QT.versit.imports = $$QT_MODULE_IMPORT_BASE
QT.versit.depends = core
QT.versit.DEFINES = QT_ADDON_VERSIT_LIB

QT_CONFIG += versit
