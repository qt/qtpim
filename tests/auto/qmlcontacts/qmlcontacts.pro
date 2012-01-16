TEMPLATE=app
TARGET=tst_qmlcontacts
SOURCES += tst_qmlcontacts.cpp
OTHER_FILES += \
    tst_contact_add_detail.qml \
    tst_contact_addresses.qml \
    tst_contact_detail_access.qml \
    tst_contactdetail.qml \
    tst_contact_emails.qml \
    tst_contact_extended_detail_e2e.qml \
    tst_contact_extendeddetails.qml \
    tst_contact_modification.qml \
    tst_contact_organizations.qml \
    tst_contact_phonenumbers.qml \
    tst_contactrelationship.qml \
    tst_contact_remove_detail.qml \
    tst_contacts_clear_details_e2e.qml \
    tst_contacts_details_saving_e2e.qml \
    tst_contacts_e2e.qml \
    tst_contacts_fetch_contacts_e2e.qml \
    tst_contacts_filtering_by_detail_e2e.qml \
    tst_contacts_filtering_e2e.qml \
    tst_contact_signals.qml \
    tst_contacts_jsondb_to_model_notification_e2e.qml \
    tst_contacts_model_to_model_notification_e2e.qml \
    tst_contacts_remove_contacts_e2e.qml \
    tst_contacts_remove_detail_e2e.qml \
    tst_contacts_save_contact_e2e.qml \
    tst_contacts_sorting_e2e.qml \
    tst_contact_urls.qml
QT += qmltest contacts versit
