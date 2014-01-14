TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS = contacts organizer versit versitorganizer plugins
qtHaveModule(qml): SUBDIRS += imports
