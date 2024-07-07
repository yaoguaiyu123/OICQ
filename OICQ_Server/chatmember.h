#pragma once

#include <QObject>
// 用户类

#pragma once

#include <QObject>
#include <QList>

// 用户类
class ChatMember : public QObject
{
    Q_OBJECT
public:
    explicit ChatMember(QObject* parent = nullptr);
    void addFriend(ChatMember* newFriend);
    bool removeFriend(ChatMember* exFriend);
    QList<ChatMember*> getFriends() const;
    qint64 getUserId();
private:
    QList<ChatMember*> friends; // 存储好友列表
    qint64 m_userId;
signals:
    void friendAdded(ChatMember* newFriend);
    void friendRemoved(ChatMember* exFriend);
};


