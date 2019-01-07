#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QVariantMap>
#include <QSqlDatabase>

class Database
{
public:
    Database();
    ~Database();
    
    void        connect();
    void        create();
    QVariantMap record(int id);
    void        setRecord(const QVariantMap &map);
    int         size();
    
private:
    QString         m_connection;
    QSqlDatabase    m_db;
};

#endif // DATABASE_H
