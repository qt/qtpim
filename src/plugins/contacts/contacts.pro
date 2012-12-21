TEMPLATE = subdirs

CONFIG += ordered

qtHaveModule(jsondb): SUBDIRS += jsondb

SUBDIRS += memory

#contains(mobility_modules,serviceframework): SUBDIRS += serviceactionmanager

