TEMPLATE = subdirs

# contacts
SUBDIRS += \
    qcontact \
    qcontactasync \
    qcontactdetail \
    qcontactdetails \
    qcontactfilter \
#TODO: re-enable the manager plugins test
#when it has been adapted to new Qt plugin mechanism
#    qcontactmanagerplugins \
    qcontactrelationship \
#    qdeclarativecontact

qtHaveModule(jsondb) {
    SUBDIRS += qcontactjsondb \
               qcontactjsondbconverter \
               qcontactjsondbstoragelocations \
               qcontactmanager \
               qcontactmanagerdetails

}

