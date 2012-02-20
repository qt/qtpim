TEMPLATE = subdirs

# common
SUBDIRS += \
    qlatin1constant

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

linux*: SUBDIRS += qcontactmemusage

#contains(mobility_modules,serviceframework){
#    SUBDIRS += \
#        qcontactmanagerfiltering \
#        qcontactactions
#}

# organizer
SUBDIRS += \
    qorganizercollection \
    qorganizeritem \
    qorganizeritemasync \
    qorganizeritemdetail \
    qorganizeritemdetails \
    qorganizeritemfilter \
    qorganizermanager \
    qorganizermanagerdetails \
    qorganizere2e

# versit
SUBDIRS += \
    qvcard21writer \
    qvcard30writer \
    qversitcontactexporter \
    qversitcontactimporter \
    qversitcontactplugins \
    qversitdocument \
    qversitproperty \
    qversitreader \
    qversitwriter

# versit organizer
SUBDIRS += \
    qversitorganizerexporter \
    qversitorganizerimporter \
    qversit
