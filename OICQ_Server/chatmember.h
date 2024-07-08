#pragma once

#include <QObject>
#include <QMap>

// 用户类
class ChatMember : public QObject
{
    Q_OBJECT
public:
    ChatMember(qint64 accountId, QString headpath, QString memberName, QObject* parent = nullptr);

    void addFriend(ChatMember* newFriend, int unreadCount);
    bool removeFriend(ChatMember* exFriend);
    QMap<ChatMember*, int>& getFriends();
    int getUnreadCount(ChatMember* friendMember) const;
    void setUnreadCount(ChatMember* friendMember, int count);
    void addUnread(ChatMember* friend_);
    void clearUnread(ChatMember* friend_);

    void setHeadPath(QString headpath);
    int getUnread(ChatMember* friend_);
    QString getHeadPath();
    QString getMemberName();
    qint64 getAccountId();


private:
    QMap<ChatMember*, int> friends;  // 存储好友列表及其未读消息数
    qint64 m_accountId = -1;
    QString m_headpath;
    QString m_memberName;

};




