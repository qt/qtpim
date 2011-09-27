TEMPLATE = subdirs

CONFIG += ordered

contains(QT_CONFIG, jsondb): SUBDIRS += jsondb

#wince*:SUBDIRS += wince
#maemo5 {
#    contains(maemo5-contacts_enabled, yes): SUBDIRS += maemo5
#}

#contains(mobility_modules,serviceframework): SUBDIRS += serviceactionmanager

