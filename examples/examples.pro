TEMPLATE = subdirs
SUBDIRS = \
#    qmlcontacts \
#    qmlorganizer \

!contains(QT_CONFIG, no-widgets): SUBDIRS += \
    calendardemo \
    todo
