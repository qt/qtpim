TEMPLATE = subdirs

CONFIG += ordered

contains(QT_CONFIG, jsondb): SUBDIRS += jsondb

SUBDIRS += memory

#contains(mobility_modules,serviceframework): SUBDIRS += serviceactionmanager

