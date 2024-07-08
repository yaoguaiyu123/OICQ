#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QList>
#include <QObject>
#include <QtSql>
#include <QTimer>
class ChatMember;

class DBManager : QObject {
public:
    explicit DBManager(QObject* parent = nullptr);

    ~DBManager();
    //users表
    bool insertDataUsers(qint64 accountId, const QString& username, const QString& password, const QString& headpath);
    QList<QVariantMap> queryDataUsers();
    QVariantMap queryDataUser(qint64 id);
    bool createTableUsers();
    QString queryDataUsersName(qint64 id);
    //friends表
    bool insertDataFriends(qint64 accountId, qint64 friendId);
    bool updateUserHeadpath(int accountId, const QString newHeadpath);
    QList<QVariantMap> queryDataFriends(qint64 id);
    bool queryDataFriend(qint64 accountId, qint64 friendId);
    bool createTableFriends();
    void updateFriendsTable(QHash<qint64, ChatMember*> allMembers);
    // friendRequest表
    bool createTableFriendRequests();
    QVariantMap queryFriendRequest(qint64 accountId, qint64 friendId);
    QList<QVariantMap> queryFriendRequests(qint64 friendId);
    bool insertFriendRequest(qint64 accountId, qint64 friendId);
    bool deleteFriendRequest(qint64 accountId, qint64 friendId);
    // 文件映射表的创建(files表)
    bool createTableFiles();
    bool insertIntoFiles(qint64 messageId, qint64 accountId, qint64 firendId, const QString& filename);
    QVariantMap queryFiles(qint64 messageId, qint64 accountId, qint64 friendId);
    // message表
    bool createTableMessages();
    bool insertMessage(qint64 senderId, qint64 receiverId, const QString& message,
        qint64 messageId, const QString& filename, const QString& filesize,
        const QString& messageDate, const QString& messageType);
    bool insertNormalMessage(qint64 senderId, qint64 receiverId, const QString& message,
        const QString& messageDate, const QString& messageType);
    QList<QVariantMap> queryMessages(qint64 senderId, qint64 receiverId);

    bool openDatabase(const QString& dbName);
    void closeDatabase();
    bool isTableExists(const QString& tableName);
    static DBManager& singleTon();

    QSqlDatabase m_database;
private:
    void startTransaction();
    void commitTransaction();

private:
    QTimer m_timer;
};

#endif // DBMANAGER_H
