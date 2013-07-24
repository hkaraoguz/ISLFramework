#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "bubbleprocess.h"
#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QFile>
#include <QDir>

#define DB_PATH "/home/hakan/Development/ISL/Datasets/ImageClef2012/training3.db"
//#define INVARIANTS_DB_PATH "/home/hakan/Development/ISL/Datasets/ImageClef2012/invariants.db"


class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = 0);
  //  ~DatabaseManager();

   bool openDB(QString filePath);

   void closeDB();

   bool deleteDB();

    // Type = 0:Lazer, 1:Hue, XX:Filter_Number, Number: the current bubble number-> obtained from frame number
   bool insertRowtoBubble(int type, int number, bubblePoint row);
   bool insertRowtoBubble(int type, int number, int pan, int tilt, double val);

   bool insertBubble(int type,int number, std::vector<bubblePoint> bubble);

   bool insertInvariants(int type,int number, std::vector< std::vector<double> > invariants);

   std::vector<bubblePoint> readBubble(int type, int number);

   QSqlError lastError();

private:
   QSqlDatabase db;

    
signals:
    
public slots:
    
};

#endif // DATABASEMANAGER_H
