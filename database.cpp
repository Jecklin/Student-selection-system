#include "database.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QStringList>
#include <QSqlDriver>

Database::Database()
    :m_connection("connection")
{
    
}

Database::~Database()
{
    if (m_db.open()) m_db.close();
}

void Database::connect()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", m_connection);
    m_db.setDatabaseName("studentSys.db");
    if (!m_db.open())
    {
        QMessageBox::warning(0, "error", "database open error");
    }
    
    if (!m_db.tables().contains("student")) this->create();
}

void Database::create()
{
    QSqlQuery query(m_db);
    query.prepare("create table student(id int primary key,"
                  "name varchar(20),"
                  "age int,"
                  "course varchar(30),"
                  "date varchar(50));");
    if (!query.exec())
    {
        qDebug() << query.lastError();
    }
    
    query.exec("insert into student values(0, 'Lili', 18, ' ', '2001/01/01');");
    query.exec("insert into student values(1, 'Jack', 20, ' ', '1999/01/01');");
    query.exec("insert into student values(2, 'Anna', 28, ' ', '1991/01/01');");
    query.exec("insert into student values(3, 'Tom',  30, ' ', '1989/01/01');");
    
}

QVariantMap Database::record(int id)
{
    QVariantMap result;
    QSqlQuery   query(m_db);
    
    query.prepare("select * from student where id = (:id)");
    query.bindValue(":id", id);
    if (!query.exec())
    {
        qDebug() << "record(): " << query.lastError();
    } 
    while(query.next())
    {
        result.insert("id",     query.value("id").toInt());
        result.insert("name",   query.value("name").toString());
        result.insert("age",    query.value("age").toInt());
        result.insert("course", query.value("course").toString());
        result.insert("date",   query.value("date").toString());
    }
    return result;
}

void Database::setRecord(const QVariantMap &map)
{
    //id error
    QSqlQuery query(m_db);
    int id = map.value("id").toInt();
    
    query.prepare("select * from student where id = (:id)");
    query.bindValue(":id", id);
    query.exec();
    
    while(query.next())
    {
        //delete
        query.prepare("delete from student where id = (:id)");
        query.bindValue(":id", id);
        while (!query.exec())
        {
            qDebug() << "delete: " << query.lastError();
        }
        
        //insert
        QString name    = map.value("name").toString();
        int     age     = map.value("age").toInt();
        QString course  = map.value("course").toString();
        QString date    = map.value("date").toString();
        
        query.prepare("insert into student (id, name, age, course, date)"
                      "values(?, ?, ?, ?, ?)");
        query.addBindValue(id);
        query.addBindValue(name);
        query.addBindValue(age);
        query.addBindValue(course);
        query.addBindValue(date);
        while (!query.exec())
        {
            qDebug() << "insert:" << query.lastError();
        }
    }

}

int Database::size()
{    
    int size = 0;
    QSqlQuery query(m_db);
    query.prepare("select * from student"); 
    if (!query.exec())
    {
        qDebug() << query.lastError();
    } 
    
    if (m_db.driver()->hasFeature(QSqlDriver::QuerySize))
    {
        size = query.size();
    }
    else
    {
        query.last();
        size = query.at() + 1;
    }
    return size;
}
