#include "databasemanager.h"

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
    // 使用 SQLite 数据库
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName("userdata.db");  // 数据库文件名
}

DatabaseManager::~DatabaseManager()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
}

// 单例模式获取实例
DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

// 打开数据库
bool DatabaseManager::openDatabase()
{
    if (m_database.isOpen()) {
        return true;
    }

    if (!m_database.open()) {
        qDebug() << "无法打开数据库:" << m_database.lastError().text();
        QMessageBox::critical(nullptr, "数据库错误",
            "无法连接到数据库！\n错误信息: " + m_database.lastError().text());
        return false;
    }

    return createTables();
}

// 创建用户表
bool DatabaseManager::createTables()
{
    QSqlQuery query;

    // 创建用户表
    QString createUserTable =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "phone VARCHAR(11) UNIQUE NOT NULL, "
        "password VARCHAR(100) NOT NULL, "
        "register_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ")";

    if (!query.exec(createUserTable)) {
        qDebug() << "创建用户表失败:" << query.lastError().text();
        return false;
    }

    qDebug() << "数据库表创建成功";
    return true;
}

// 检查用户是否存在
bool DatabaseManager::userExists(const QString& phone)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE phone = :phone");
    query.bindValue(":phone", phone);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}

// 注册用户
// ================ 修改注册方法 ================
bool DatabaseManager::registerUser(const QString& phone, const QString& password)
{
    // 检查用户是否已存在
    if (userExists(phone)) {
        QMessageBox::warning(nullptr, "注册失败", "该手机号已被注册！");
        return false;
    }

    // 加密密码
    QString encryptedPassword = encryptPassword(password);

    QSqlQuery query;
    query.prepare("INSERT INTO users (phone, password) VALUES (:phone, :password)");
    query.bindValue(":phone", phone);
    query.bindValue(":password", encryptedPassword);  // 存储加密后的密码

    if (!query.exec()) {
        qDebug() << "注册失败:" << query.lastError().text();
        QMessageBox::critical(nullptr, "注册失败",
            "数据库错误: " + query.lastError().text());
        return false;
    }

    qDebug() << "用户注册成功:" << phone;
    return true;
}

// ================ 修改登录方法 ================
bool DatabaseManager::loginUser(const QString& phone, const QString& password)
{
    // 加密输入的密码
    QString encryptedPassword = encryptPassword(password);

    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE phone = :phone");
    query.bindValue(":phone", phone);

    if (query.exec() && query.next()) {
        QString storedPassword = query.value(0).toString();
        // 比较加密后的密码
        return (storedPassword == encryptedPassword);
    }

    return false;
}

//密码加密
QString DatabaseManager::encryptPassword(const QString& password)
{
    QByteArray hash = QCryptographicHash::hash(
        password.toUtf8(),
        QCryptographicHash::Sha256
    );
    return QString(hash.toHex());
}
