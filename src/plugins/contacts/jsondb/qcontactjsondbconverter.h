#ifndef QCONTACTJSONDBCONVERTER_H
#define QCONTACTJSONDBCONVERTER_H

#include <QVariantMap>
#include <QHash>
#include "qcontact.h"
#include "qcontactjsondbengine.h"
#include "qcontactabstractrequest.h"
#include "qcontactfilter.h"

QTPIM_USE_NAMESPACE

class QContactJsonDbConverter
{
public:
    QContactJsonDbConverter();
    ~QContactJsonDbConverter();
    bool toQContact(const QVariantMap& object, QContact* contact, const QContactJsonDbEngine& engine);
    bool toQContacts(const QVariantList& jsonObjects, QList<QContact>& convertedContacts, const QContactJsonDbEngine& engine, QContactManager::Error& error);
    bool toJsonContact(QVariantMap* object, const QContact& contact);
    bool updateContexts(const QVariantMap& object, QContactDetail* detail);
    bool updateContexts(const QContactDetail& detail, QVariantMap* object);
    QString queryFromRequest(QContactAbstractRequest* request);
    QString convertFilter(const QContactFilter& filter) const;
    QString convertSortOrder(const QList<QContactSortOrder>& sortOrders) const;
    QContactId convertId(const QString& id, const QContactJsonDbEngine& engine) const;
    QString convertId(const QContactLocalId& id) const;
    QHash<QString, QString> detailsToJsonMapping;
    QHash<QString, QString> contactNameFieldsMapping;
    QHash<QString, QString> organizationFieldsMapping;
    QHash<QString, QString> addressFieldsMapping;
    QHash<QString, QString> phoneNumbersSubtypesMapping;
private:
    void initializeMappings();
    void createMatchFlagQuery(QString& queryString, QContactFilter::MatchFlags flags, const QString& value, const QString& UriScheme = "");
};
#endif // QCONTACTJSONDBCONVERTER_H
