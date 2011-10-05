TEMPLATE = subdirs

# common
SUBDIRS += \
    qlatin1constant \
    qmalgorithms

# contacts
SUBDIRS += \
    qcontact \
#    qcontactasync \
    qcontactdetail \
    qcontactdetaildefinition \
    qcontactdetails \
    qcontactfilter \
#    qcontactmanager \
#    qcontactmanagerdetails \
    qcontactmanagerplugins \
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
    qorganizere2e \
#    qdeclarativeorganizer

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
