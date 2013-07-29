#include "databasemanager.h"
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QDebug>



//static QSqlDatabase bubbledb;
//static QSqlDatabase invariantdb;

static QSqlDatabase db;

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{
}
bool DatabaseManager::openDB(QString filePath)
{
    if(!db.isOpen())
    {
        // Find QSLite driver
        db = QSqlDatabase::addDatabase("QSQLITE");

        #ifdef Q_OS_LINUX
        // NOTE: We have to store database file into user home folder in Linux
        //QString path(QDir::home().path());

        //path.append(QDir::separator()).append("Development").append(QDir::separator()).append("ISL").append(QDir::separator()).append("Datasets").append(QDir::separator()).append("ImageClef2012").append(QDir::separator()).append("bubble.bubbledb");
        QString path = QDir::toNativeSeparators(filePath);

        db.setDatabaseName(path);
        //Development/ISL/Datasets/ImageClef2012
        #else
        // NOTE: File exists in the application private folder, in Symbian Qt implementation
        //  bubbledb.setDatabaseName("my.bubbledb.sqlite");
        #endif

        // Open databasee
        return db.open();
    }

    return true;
}
/*DatabaseManager::~DatabaseManager()
{

    if(bubbledb.isOpen()) bubbledb.close();
}*/

QSqlError DatabaseManager::lastError()
{
    // If opening database has failed user can ask
    // error description by QSqlError::text()
    return db.lastError();

}
void DatabaseManager::closeDB()
{
    if(db.isOpen()) db.close();
}

bool DatabaseManager::deleteDB()
{
    // Close database
    db.close();

#ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("my.bubbledb.sqlite");
    path = QDir::toNativeSeparators(path);
    return QFile::remove(path);
#else

    // Remove created database binary file
    return QFile::remove("my.bubbledb.sqlite");
#endif

}
bool DatabaseManager::insertRowtoBubble(int type, int number, int pan, int tilt, double val)
{
    bool ret = false;

    if (db.isOpen())
    {
        //http://www.sqlite.org/autoinc.html
        // NULL = is the keyword for the autoincrement to generate next value

        QSqlQuery query;

        bool exists = query.exec(QString("select * from bubble where type = %1 and number = %2 and pan = %3 and tilt = %4").arg(type).arg(number).arg(pan).arg(tilt));

        if(exists){
            if(!query.next())
                ret = query.exec(QString("insert into bubble values('%1', '%2', '%3', '%4', %5)").arg(type).arg(number).arg(pan).arg(tilt).arg(val));
            else
                ret = query.exec(QString("update bubble set val = %5 where type = %1 and number = %2 and pan = %3 and tilt = %4").arg(type).arg(number).arg(pan).arg(tilt).arg(val));
        }

    }

    return ret;
}
bool DatabaseManager::insertRowtoBubble(int type, int number, bubblePoint row)
{
    bool ret = false;

    if (db.isOpen())
    {
        //http://www.sqlite.org/autoinc.html
        // NULL = is the keyword for the autoincrement to generate next value

        QSqlQuery query;

        bool exists = query.exec(QString("select * from bubble where type = %1 and number = %2 and pan = %3 and tilt = %4").arg(type).arg(number).arg(row.panAng).arg(row.tiltAng));

        if(exists)
        {
            if(!query.next())
                ret = query.exec(QString("insert into bubble values('%1', '%2', '%3', '%4', %5)").arg(type).arg(number).arg(row.panAng).arg(row.tiltAng).arg(row.val));
            else
                ret = query.exec(QString("update bubble set val = %5 where type = %1 and number = %2 and pan = %3 and tilt = %4").arg(type).arg(number).arg(row.panAng).arg(row.tiltAng).arg(row.val));
        }

    }

    return ret;
}
std::vector<bubblePoint> DatabaseManager::readBubble(int type, int number)
{
    QSqlQuery query(QString("select * from bubble where type = %1 and number = %2").arg(type).arg(number));

    std::vector<bubblePoint> bubble;

    while(query.next())
    {
        bubblePoint pt ;

        pt.panAng = query.value(2).toInt();

        pt.tiltAng = query.value(3).toInt();

        pt.val = query.value(4).toDouble();

        bubble.push_back(pt);

        //qDebug()<<"value is"<<query.value(4).toDouble();
    }
    // select * from bubble where number = 2 and type = 0


    return bubble;
}
bool DatabaseManager::insertBubble(int type, int number, std::vector<bubblePoint> bubble)
{

    bool ret = false;

    if (db.isOpen())
    {

        QSqlQuery query;
        QSqlQuery updateQuery;

        // First check, if a bubble has already entered to the table
     /*   bool exists = query.exec(QString("select val from bubble where type = %1 and number = %2 LIMIT 2").arg(type).arg(number));

        // If query is successfully executed
        if(exists)
        {
            // if there are elements received from the table, then there exists a bubble, we should delete those entries
            if(query.next())
            {
                ret = query.exec(QString("delete from bubble where type = %1 and number = %2").arg(type).arg(number));

                // If deletion is not successfuly executed return false
                if(!ret)

                    return false;

            }*/

            //else return false;

            // Speed up the multiple-row insertion by using transactions
            //query.exec(QString("BEGIN TRANSACTION"));

          //  query.exec("PRAGMA journal_mode = MEMORY");
         //  query.exec("PRAGMA synchronous = OFF");
            query.prepare(QString("replace into bubble values( ?, ?, ?, ?, ?)"));

                          //    query.prepare(QString("insert into bubble values( :type, :number, :pan, :tilt, :val) ON DUPLICATE KEY update val = :val)"));

                          //    updateQuery.prepare("update bubble set val = :val where type = %type and number = %2 and pan = %3 and tilt = %4");
            QVariantList typee;
            QVariantList numberr;
            QVariantList pan;
            QVariantList tilt;
            QVariantList val;
            db.transaction();
            // Insert new bubble
            for(int i = 0; i <bubble.size(); i++)
            {

                bubblePoint pt;

                pt = bubble.at(i);

                typee<<type;
                numberr<<number;
                pan<<pt.panAng;
                tilt<<pt.tiltAng;
                val<<pt.val;

          /*      query.bindValue(":type",type);
                 query.bindValue(":number",number);
                  query.bindValue(":pan",pt.panAng);
                   query.bindValue(":tilt",pt.tiltAng);
                   query.bindValue(":val",pt.val);*/
                 //  query.exec();

                //query.exec(QString("insert into bubble values('%1', '%2', '%3', '%4', %5)").arg(type).arg(number).arg(pt.panAng).arg(pt.tiltAng).arg(pt.val));


            }

            query.addBindValue(typee);
            query.addBindValue(numberr);
            query.addBindValue(pan);
            query.addBindValue(tilt);
            query.addBindValue(val);


            if (!query.execBatch())
                 qDebug() << query.lastError();
            db.commit();
            //query.exec(QString("COMMIT TRANSACTION"));

            return true;

        }
       /* else
        {

            // Speed up the multiple-row insertion by using transactions
            query.exec(QString("BEGIN TRANSACTION"));

            // Insert new bubble
            for(int i = 0; i <bubble.size(); i++)
            {

                bubblePoint pt;

                pt = bubble.at(i);

                query.exec(QString("insert into bubble values('%1', '%2', '%3', '%4', %5)").arg(type).arg(number).arg(pt.panAng).arg(pt.tiltAng).arg(pt.val));


            }
            query.exec(QString("END TRANSACTION"));

            return true;

        }*/


        return false;

  //  }

}
bool DatabaseManager::insertInvariants(int type, int number, std::vector< std::vector<double> > invariants)
{

    bool ret = false;

    if (db.isOpen())
    {

        QSqlQuery query;

        // First check, if a bubble has already entered to the table
    /*    bool exists = query.exec(QString("select * from invariant where type = %1 and number = %2").arg(type).arg(number));

        // If query is successfully executed
        if(exists)
        {
            // if there are elements received from the table, then there exists a bubble, we should delete those entries
            if(query.next())
            {
                ret = query.exec(QString("delete from invariant where type = %1 and number = %2").arg(type).arg(number));

                // If deletion is not successfuly executed return false
                if(!ret)

                    return false;

            }
*/
            //else return false;

            // Speed up the multiple-row insertion by using transactions
            query.exec(QString("BEGIN TRANSACTION"));

            // Insert new bubble
            for(int i = 0; i <invariants.size(); i++)
            {
                for(int j = 0; j < invariants[i].size();j++){


                    double val = invariants[i][j];

                    query.exec(QString("replace into invariant values('%1', '%2', '%3')").arg(type).arg(number).arg(val));


                }
            }
            query.exec(QString("COMMIT TRANSACTION"));

            return true;

        }



  //  }

    return false;

}
