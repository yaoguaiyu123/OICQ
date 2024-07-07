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
    QList<ChatMember*> getFriends() const;
    int getUnreadCount(ChatMember* friendMember) const;
    void setUnreadCount(ChatMember* friendMember, int count);
    qint64 getAccountId() const;

    void addUnread(ChatMember* friend_);
    void clearUnread(ChatMember* friend_);
    int getUnread(ChatMember* friend_);

private:
    QMap<ChatMember*, int> friends;  // 存储好友列表及其未读消息数
    qint64 m_accountId = -1;
    QString m_headpath;
    QString m_memberName;

};




