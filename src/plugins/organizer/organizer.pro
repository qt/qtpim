TEMPLATE = subdirs

CONFIG += ordered

qtHaveModule(jsondb): SUBDIRS += jsondb

# Only compile this for tests (to make sure it compiles).. don't deploy this
contains(QT_BUILD_PARTS,tests): SUBDIRS += skeleton

SUBDIRS += memory
