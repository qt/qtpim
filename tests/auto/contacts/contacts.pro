TEMPLATE = subdirs

# contacts
SUBDIRS += \
    qcontact \
    qcontactasync \
    qcontactdetail \
    qcontactdetails \
    qcontactfilter \
#    qcontactmanager \
    qcontactmanagerdetails \
#TODO: re-enable the manager plugins test
#when it has been adapted to new Qt plugin mechanism
#    qcontactmanagerplugins \
    qcontactrelationship \
#    qdeclarativecontact

#contains(QT_CONFIG, jsondb) {
#    SUBDIRS += qcontactjsondb \
#               qcontactjsondbconverter
#}

