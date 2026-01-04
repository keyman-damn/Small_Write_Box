#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QCryptographicHash>
class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    static DatabaseManager& instance();

    bool openDatabase();
    bool createTables();

    // 用户相关操作
    bool registerUser(const QString& phone, const QString& password);
    bool loginUser(const QString& phone, const QString& password);
    bool userExists(const QString& phone);

private:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    QString encryptPassword(const QString& password);
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_database;
};

#endif // DATABASEMANAGER_H
