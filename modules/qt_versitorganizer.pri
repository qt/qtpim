QT.versitorganizer.VERSION = 2.0.0
QT.versitorganizer.MAJOR_VERSION = 2
QT.versitorganizer.MINOR_VERSION = 0
QT.versitorganizer.PATCH_VERSION = 0

QT.versitorganizer.name = QtVersitOrganizer
QT.versitorganizer.bins = $$QT_MODULE_BIN_BASE
QT.versitorganizer.includes = $$QT_MODULE_INCLUDE_BASE $$QT_MODULE_INCLUDE_BASE/QtVersitOrganizer
QT.versitorganizer.private_includes = $$QT_MODULE_INCLUDE_BASE/QtVersitOrganizer/$$QT.versit.VERSION
QT.versitorganizer.sources = $$QT_MODULE_BASE/src/versitorganizer
QT.versitorganizer.libs = $$QT_MODULE_LIB_BASE
QT.versitorganizer.plugins = $$QT_MODULE_PLUGIN_BASE
QT.versitorganizer.imports = $$QT_MODULE_IMPORT_BASE
QT.versitorganizer.depends = core
QT.versitorganizer.DEFINES = QT_VERSITORGANIZER_LIB

QT_CONFIG += versitorganizer
