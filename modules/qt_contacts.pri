QT.contacts.VERSION = 2.0.0
QT.contacts.MAJOR_VERSION = 2
QT.contacts.MINOR_VERSION = 0
QT.contacts.PATCH_VERSION = 0

QT.contacts.name = QtAddOnContacts
QT.contacts.bins = $$QT_MODULE_BIN_BASE
QT.contacts.includes = $$QT_MODULE_INCLUDE_BASE $$QT_MODULE_INCLUDE_BASE/QtAddOnContacts
QT.contacts.private_includes = $$QT_MODULE_INCLUDE_BASE/QtAddOnContacts/$$QT.contacts.VERSION
QT.contacts.sources = $$QT_MODULE_BASE/src/contacts
QT.contacts.libs = $$QT_MODULE_LIB_BASE
QT.contacts.plugins = $$QT_MODULE_PLUGIN_BASE
QT.contacts.imports = $$QT_MODULE_IMPORT_BASE
QT.contacts.depends = core
QT.contacts.DEFINES = QT_ADDON_CONTACTS_LIB

QT_CONFIG += contacts
