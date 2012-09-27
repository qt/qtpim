TEMPLATE = subdirs
SUBDIRS +=

!contains(QT_CONFIG, no-widgets): SUBDIRS += \
    calendardemo \
    todo
