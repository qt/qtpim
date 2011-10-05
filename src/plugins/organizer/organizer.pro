TEMPLATE = subdirs

CONFIG += ordered

contains(QT_CONFIG, jsondb): SUBDIRS += jsondb

#simulator:SUBDIRS += simulator

# Only compile this for tests (to make sure it compiles).. don't deploy this
contains(QT_BUILD_PARTS,tests): SUBDIRS += skeleton
