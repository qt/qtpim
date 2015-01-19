TEMPLATE = subdirs

# contacts
SUBDIRS += \
    qcontact \
    qcontactasync \
    qcontactcollection \
    qcontactdetail \
    qcontactdetails \
    qcontactfilter \
    qcontactsortorder \
#TODO: re-enable the manager plugins test
#when it has been adapted to new Qt plugin mechanism
#    qcontactmanagerplugins \
    qcontactrelationship \
#    qdeclarativecontact
    qcontactmanager \
    qcontactmanagerdetails \
    qcontactmanagerfiltering \
    qmlcontacts

