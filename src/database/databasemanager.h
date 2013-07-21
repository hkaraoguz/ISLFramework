#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QFile>
#include <QDir>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = 0);
    ~DatabaseManager();

    bool openDB();

    bool deleteDB();

    // Type = 0:Lazer, 1:Hue, XX:Filter_Number, Number: the current bubble number-> obtained from frame number
    void insertRowtoBubble(int type, int number, double pan, double tilt, double val);

    void readBubble(int type, int number);

    QSqlError lastError();

private:
      QSqlDatabase db;
    
signals:
    
public slots:
    
};

#endif // DATABASEMANAGER_H
