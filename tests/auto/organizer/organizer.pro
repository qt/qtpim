TEMPLATE = subdirs

SUBDIRS += \
    qorganizercollection \
    qorganizeritem \
    qorganizeritemasync \
    qorganizeritemdetail \
    qorganizeritemdetails \
    qorganizeritemfilter \
    qorganizeritemsortorder \
    qorganizermanager \
    qorganizermanagerdetails \
    qorganizere2e

qtHaveModule(qmltest): SUBDIRS += qmlorganizer
