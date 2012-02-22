TEMPLATE = subdirs
SUBDIRS += maliciousplugin \
           unittest

contains(QT_CONFIG, jsondb): SUBDIRS += jsondbtests
