#ifndef QCONTACTJSONDBBACKUP_H
#define QCONTACTJSONDBBACKUP_H

class QContactJsonDbBackup
{
public:
    QContactJsonDbBackup();
    ~QContactJsonDbBackup();
    bool loadTestData();
private:
    bool backupJsonDb();
    bool clearJsonDb();
    bool revertJsonDb();
    bool doRequest(const QVariantList& objects, bool isInsert);
int  wasteSomeTime();
    QVariantList m_backupData;
};

#endif // QCONTACTJSONDBBACKUP_H
