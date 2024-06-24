#include "dbmanager.h"

DBManager::DBManager(QObject* parent)
    : QObject { parent }
{
    openDatabase("/root/.config/OICQ/server/database/copyQQ.db");   //open数据库
}

DBManager::~DBManager()
{
    m_database.close();
}

// 在数据库中添加数据
bool DBManager::insertDataUsers(qint64 accountId, const QString& username, const QString& password, const QString& headpath)
{
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return false;
    }

    QSqlQuery query(m_database);
    query.prepare("INSERT INTO users (accountid, username, password, headpath) VALUES (?, ?, ?, ?)");
    query.addBindValue(accountId);
    query.addBindValue(username);
    query.addBindValue(password);
    query.addBindValue(headpath);
    if (!query.exec()) {
        qDebug() << "Error: Failed to insert data:" << query.lastError().text();
        return false;
    }

    return true;
}

// 查询数据表users
QList<QVariantMap> DBManager::queryDataUsers()
{
    QList<QVariantMap> result;

    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return result;
    }

    QSqlQuery query(m_database);
    if (!query.exec("SELECT * FROM users")) {
        qDebug() << "Error: Failed to query data:" << query.lastError().text();
        return result;
    }

    while (query.next()) {
        QVariantMap row;
        row["accountid"] = query.value("accountid").toLongLong();
        row["username"] = query.value("username").toString();
        row["password"] = query.value("password").toString();
        row["headpath"] = query.value("headpath").toString();
        result.append(row);
    }

    return result;
}

//查询单独的信息
QVariantMap DBManager::queryDataUser(qint64 id){
    QVariantMap row;

    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return row;
    }

    QSqlQuery query(m_database);
    if (!query.exec(QString("SELECT * FROM users WHERE accountid = %1").arg(id))) {
        qDebug() << "Error: Failed to query data:" << query.lastError().text();
        return row;
    }

    while (query.next()) {
        row["accountid"] = query.value("accountid").toLongLong();
        row["username"] = query.value("username").toString();
        row["password"] = query.value("password").toString();
        row["headpath"] = query.value("headpath").toString();
    }
    return row;
}

// 查询名字
QString DBManager::queryDataUsersName(qint64 id)
{
    QString name;
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return "";
    }

    QSqlQuery query(m_database);
    if (!query.exec(QString("SELECT * FROM users WHERE accountid = %1").arg(id))) {
        qDebug() << "Error: Failed to query data:" << query.lastError().text();
        return "";
    }

    while (query.next()) {
        name = query.value("username").toString();
    }

    return name;
}

// 创建数据库user表
bool DBManager::createTableUsers()
{
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return false;
    }

    QSqlQuery query(m_database);
    QString createSql = "CREATE TABLE IF NOT EXISTS users ("
                        "id INTEGER PRIMARY KEY AUTObenINCREMENT, "
                        "accountid INTEGER UNIQUE, "
                        "username TEXT, "
                        "password TEXT, "
                        "headpath TEXT)";
    if (!query.exec(createSql)) {
        qDebug() << "Error: Failed to create table:" << query.lastError().text();
        return false;
    }

    return true;
}

// 更新头像
bool DBManager::updateUserHeadpath(int accountId, const QString newHeadpath)
{
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return false;
    }

    QSqlQuery query(m_database);
    query.prepare("UPDATE users SET headpath = :headpath WHERE accountid = :accountid");
    query.bindValue(":headpath", newHeadpath);
    query.bindValue(":accountid", accountId);

    if (!query.exec()) {
        qDebug() << "Error: Failed to update headpath:" << query.lastError().text();
        return false;
    }

    return true;
}


bool DBManager::insertDataFriends(qint64 accountId, qint64 friendId){
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return false;
    }

    QSqlQuery query(m_database);
    query.prepare("INSERT INTO friends (accountid, friendid) VALUES (?, ?)");
    query.addBindValue(accountId);
    query.addBindValue(friendId);
    if (!query.exec()) {
        qDebug() << "Error: Failed to insert data:" << query.lastError().text();
        return false;
    }

    return true;
}

QList<QVariantMap> DBManager::queryDataFriends(qint64 id){
    QList<QVariantMap> result;

    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return result;
    }

    QSqlQuery query(m_database);
    if (!query.exec(QString("SELECT * FROM friends WHERE accountid = %1").arg(id))) {
        qDebug() << "Error: Failed to query data:" << query.lastError().text();
        return result;
    }

    while (query.next()) {
        QVariantMap row;
        row["accountid"] = query.value("accountid").toLongLong();
        row["friendid"] = query.value("friendid").toLongLong();
        result.append(row);
    }
    return result;
}

bool DBManager::queryDataFriend(qint64 accountId ,qint64 friendId){
    QVariantMap row;
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return false;
    }

    QSqlQuery query(m_database);
    QString selectSql = "SELECT * FROM friends WHERE accountid = :accountId AND friendid = :friendId";
    query.prepare(selectSql);
    query.bindValue(":accountId", accountId);
    query.bindValue(":friendId", friendId);

    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        row["id"] = query.value("id");
        row["accountId"] = query.value("accountid");
        row["friendId"] = query.value("friendid");
    } else {
        qDebug() << "No friend request found for the provided account ID and friend ID";
    }

    return !row.isEmpty();
}

bool DBManager::createTableFriends(){
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return false;
    }

    QSqlQuery query(m_database);
    QString createSql = "CREATE TABLE IF NOT EXISTS friends ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "accountid INTEGER, "
                        "friendid INTEGER)";
    if (!query.exec(createSql)) {
        qDebug() << "Error: Failed to create table:" << query.lastError().text();
        return false;
    }

    return true;
}

// 返回单例对象
DBManager& DBManager::singleTon()
{
    static DBManager dao;
    return dao;
}

bool DBManager::openDatabase(const QString& dbName)
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(dbName);

    if (!m_database.open()) {
        qDebug() << "Error: Failed to open database" << dbName << ":"
                 << m_database.lastError().text();
        return false;
    }

    return true;
}

void DBManager::closeDatabase()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
}

bool DBManager::isTableExists(const QString& tableName){
    QStringList tables = m_database.tables();
    return tables.contains(tableName);
}

// 创建好友请求表
bool DBManager::createTableFriendRequests()
{
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return false;
    }

    QSqlQuery query(m_database);
    QString createSql = "CREATE TABLE IF NOT EXISTS friendRequests ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "accountid INTEGER, "
                        "friendid INTEGER)";
    if (!query.exec(createSql)) {
        qDebug() << "Error: Failed to create table:" << query.lastError().text();
        return false;
    }

    return true;
}

// 查询一个好友请求
QVariantMap DBManager::queryFriendRequest(qint64 accountId, qint64 friendId)
{
    QVariantMap result;
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return result;
    }

    QSqlQuery query(m_database);
    QString selectSql = "SELECT * FROM friendRequests WHERE accountid = :accountId AND friendid = :friendId";
    query.prepare(selectSql);
    query.bindValue(":accountId", accountId);
    query.bindValue(":friendId", friendId);

    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query:" << query.lastError().text();
        return result;
    }

    if (query.next()) {
        result["id"] = query.value("id");
        result["accountId"] = query.value("accountid");
        result["friendId"] = query.value("friendid");
    } else {
        qDebug() << "No friend request found for the provided account ID and friend ID";
    }

    return result;
}

// 查询接收到的多个好友请求
QList<QVariantMap> DBManager::queryFriendRequests(qint64 friendId)
{
    QList<QVariantMap> list;
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return list;
    }

    QSqlQuery query(m_database);
    QString selectSql = "SELECT * FROM friendRequests WHERE friendid = :friendId";
    query.prepare(selectSql);
    query.bindValue(":friendId", friendId);

    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query:" << query.lastError().text();
        return list;
    }

    while (query.next()) {
        QVariantMap result;
        result["id"] = query.value("id");
        result["accountId"] = query.value("accountid");
        result["friendId"] = query.value("friendid");
        list.append(result);
    }

    if (list.isEmpty()) {
        qDebug() << "No friend request found for the provided friend ID";
    }

    return list;
}


//插入好友请求
bool DBManager::insertFriendRequest(qint64 accountId, qint64 friendId) {
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return false;
    }

    QSqlQuery query(m_database);
    QString insertSql = "INSERT INTO friendRequests (accountid, friendid) VALUES (:accountId, :friendId)";
    query.prepare(insertSql);
    query.bindValue(":accountId", accountId);
    query.bindValue(":friendId", friendId);

    if (!query.exec()) {
        qDebug() << "Error: Failed to insert friend request:" << query.lastError().text();
        return false;
    }

    return true;
}

//删除好友请求
bool DBManager::deleteFriendRequest(qint64 accountId, qint64 friendId) {
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return false;
    }

    QSqlQuery query(m_database);
    QString deleteSql = "DELETE FROM friendRequests WHERE accountid = :accountId AND friendid = :friendId";
    query.prepare(deleteSql);
    query.bindValue(":accountId", accountId);
    query.bindValue(":friendId", friendId);

    if (!query.exec()) {
        qDebug() << "Error: Failed to delete friend request:" << query.lastError().text();
        return false;
    }

    return true;
}


// 文件表的创建函数
bool DBManager::createTableFiles() {
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return false;
    }

    QSqlQuery query(m_database);
    QString createSql = "CREATE TABLE IF NOT EXISTS files ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "messageId INTEGER, "
                        "accountId INTEGER, "
                        "friendId INTEGER, "
                        "filename TEXT)";
    if (!query.exec(createSql)) {
        qDebug() << "Error: Failed to create table:" << query.lastError().text();
        return false;
    }

    return true;
}


// 文件表新增函数
bool DBManager::insertIntoFiles(qint64 messageId, qint64 accountId, qint64 friendId, const QString& filename) {
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return false;
    }

    QSqlQuery query(m_database);
    query.prepare("INSERT INTO files (messageId, accountId, friendId, filename) VALUES (:messageId, :accountId, :friendId, :filename)");
    query.bindValue(":messageId", messageId);
    query.bindValue(":accountId", accountId);
    query.bindValue(":friendId", friendId);
    query.bindValue(":filename", filename);

    if (!query.exec()) {
        qDebug() << "Error: Failed to insert data into files table:" << query.lastError().text();
        return false;
    }

    return true;
}

// 文件表查询函数
QVariantMap DBManager::queryFiles(qint64 messageId, qint64 accountId, qint64 friendId) {
    QVariantMap result;
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return result;
    }
    QSqlQuery query(m_database);
    query.prepare("SELECT * FROM files WHERE messageId = :messageId AND accountId = :accountId AND friendId = :friendId");
    query.bindValue(":messageId", messageId);
    query.bindValue(":accountId", accountId);
    query.bindValue(":friendId", friendId);

    if (!query.exec()) {
        qDebug() << "Error: Failed to query data from files table:" << query.lastError().text();
        return result;
    }
    if (query.first()) {
        QSqlRecord record = query.record();
        for (int i = 0; i < record.count(); i++) {
            result[record.fieldName(i)] = query.value(i);
        }
    }
    return result;
}

/** messages begin
 *  记录历史聊天消息的数据表
*/
// 创建消息表格
bool DBManager::createTableMessages() {
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return false;
    }

    QSqlQuery query(m_database);
    bool success = query.exec(
        "CREATE TABLE IF NOT EXISTS messages ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "senderId INTEGER, "
        "receiverId INTEGER, "
        "message TEXT, "
        "messageId INTEGER, "
        "filename TEXT DEFAULT '', "
        "filesize TEXT DEFAULT '', "
        "messageDate TEXT, "
        "messageType TEXT),"
        "is_read INTEGER"
        );

    if (!success) {
        qDebug() << "Error: Failed to create messages table:" << query.lastError().text();
        return false;
    }

    return true;
}

// 插入文件消息(messages)
bool DBManager::insertMessage(qint64 senderId, qint64 receiverId, const QString& message,
    qint64 messageId, const QString& filename, const QString& filesize,
    const QString& messageDate, const QString& messageType) {
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return false;
    }

    QSqlQuery query(m_database);
    query.prepare("INSERT INTO messages (senderId, receiverId, message, messageId, filename, filesize, messageDate, messageType) "
                  "VALUES (:senderId, :receiverId, :message, :messageId, :filename, :filesize, :messageDate, :messageType)");
    query.bindValue(":senderId", senderId);
    query.bindValue(":receiverId", receiverId);
    query.bindValue(":message", message);
    query.bindValue(":messageId", messageId);
    query.bindValue(":filename", filename);
    query.bindValue(":filesize", filesize);
    query.bindValue(":messageDate", messageDate);
    query.bindValue(":messageType", messageType);

    if (!query.exec()) {
        qDebug() << "Error: Failed to insert data:" << query.lastError().text();
        return false;
    }

    return true;
}


// 插入普通消息(messages)
bool DBManager::insertNormalMessage(qint64 senderId, qint64 receiverId, const QString& message,
    const QString& messageDate, const QString& messageType) {
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return false;
    }

    QSqlQuery query(m_database);
    query.prepare("INSERT INTO messages (senderId, receiverId, message, messageDate, messageType) "
                  "VALUES (:senderId, :receiverId, :message, :messageDate, :messageType)");
    query.bindValue(":senderId", senderId);
    query.bindValue(":receiverId", receiverId);
    query.bindValue(":message", message);
    query.bindValue(":messageDate", messageDate);
    query.bindValue(":messageType", messageType);

    if (!query.exec()) {
        qDebug() << "Error: Failed to insert data:" << query.lastError().text();
        return false;
    }

    return true;
}

// 查询两个人的聊天记录(messages)
QList<QVariantMap> DBManager::queryMessages(qint64 senderId, qint64 receiverId) {
    QList<QVariantMap> result;
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return result;
    }

    QSqlQuery query(m_database);
    query.prepare("SELECT * FROM messages WHERE senderId = :senderId AND receiverId = :receiverId");
    query.bindValue(":senderId", senderId);
    query.bindValue(":receiverId", receiverId);

    if (query.exec()) {
        while (query.next()) {
            QVariantMap row;
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); i++) {
                row[record.fieldName(i)] = query.value(i);
            }
            result.append(row);
        }
    } else {
        qDebug() << "Error: Failed to query data:" << query.lastError().text();
    }

    return result;
}

// 更新两个人的聊天消息的已读未读的状态(将未读更新为已读)
bool DBManager::updateIsRead(qint64 senderId, qint64 receiverId) {
    if (!m_database.isOpen()) {
        qDebug() << "Error: Database is not opened";
        return false;
    }

    QSqlQuery query(m_database);
    query.prepare("UPDATE messages SET is_read = 1 WHERE senderId = :senderId AND receiverId = :receiverId AND is_read = 0");
    query.bindValue(":senderId", senderId);
    query.bindValue(":receiverId", receiverId);

    bool success = query.exec();
    if (!success) {
        qDebug() << "Error: Failed to update messages as read:" << query.lastError().text();
        return false;
    }

    return true;
}

/// messages end



