#include "databasemanager.h"
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QDebug>
DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{
}
bool DatabaseManager::openDB()
{
    // Find QSLite driver
    db = QSqlDatabase::addDatabase("QSQLITE");

#ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("Development").append(QDir::separator()).append("ISL").append(QDir::separator()).append("Datasets").append(QDir::separator()).append("ImageClef2012").append(QDir::separator()).append("bubble.db");
    path = QDir::toNativeSeparators(path);
    db.setDatabaseName(path);
    //Development/ISL/Datasets/ImageClef2012
#else
    // NOTE: File exists in the application private folder, in Symbian Qt implementation
    db.setDatabaseName("my.db.sqlite");
#endif

    // Open databasee
    return db.open();
}
DatabaseManager::~DatabaseManager()
{

    if(db.isOpen()) db.close();
}

QSqlError DatabaseManager::lastError()
{
    // If opening database has failed user can ask
    // error description by QSqlError::text()
    return db.lastError();

}

bool DatabaseManager::deleteDB()
{
    // Close database
    db.close();

#ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("my.db.sqlite");
    path = QDir::toNativeSeparators(path);
    return QFile::remove(path);
#else

    // Remove created database binary file
    return QFile::remove("my.db.sqlite");
#endif

}
void DatabaseManager::insertRowtoBubble(int type, int number, double pan, double tilt, double val)
{
    if (db.isOpen())
    {
           //http://www.sqlite.org/autoinc.html
           // NULL = is the keyword for the autoincrement to generate next value

           QSqlQuery query;
           bool ret = query.exec(QString("insert into bubble values('%1', '%2', '%3', '%4', %5)").arg(type).arg(number).arg(pan).arg(tilt).arg(val));

           // Get database given autoincrement value
           if (ret)
               {
              // query.lastInsertId().toInt();
               // http://www.sqlite.org/c3ref/last_insert_rowid.html
            // int  newId = query.lastInsertId().toInt();
               }

           }
}
void DatabaseManager::readBubble(int type, int number)
{
      QSqlQuery query(QString("select * from bubble where type = %1 and number = %2").arg(type).arg(number));

      while(query.next())
      {

          qDebug()<<"value is"<<query.value(4).toDouble();
      }
   // select * from bubble where number = 2 and type = 0


}
