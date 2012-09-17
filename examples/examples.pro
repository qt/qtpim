TEMPLATE = subdirs
SUBDIRS =
#   contacts \
#    qmlcontacts \
#    qmlorganizer \

!contains(QT_CONFIG, no-widgets): SUBDIRS += \
    calendardemo \
    todo
