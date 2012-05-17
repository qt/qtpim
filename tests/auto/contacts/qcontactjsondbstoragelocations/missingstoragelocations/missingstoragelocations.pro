TEMPLATE = subdirs

contains(QT_CONFIG, jsondb): SUBDIRS +=nosystemstorage nouserdatastorage nostorages

