TEMPLATE = subdirs
SUBDIRS +=

qtHaveModule(widgets): SUBDIRS += \
    calendardemo \
    todo
