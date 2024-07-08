#pragma once

#include <QObject>
#include "chatmember.h"
#include <QHash>
#include <QTimer>

// 存放所有用户的数据类
class ChatMemberData : public QObject
{
    Q_OBJECT
public:
    ChatMemberData(QObject* parent = nullptr);
    static ChatMemberData& singleTon();

    // 好友表作为经常需要访问的表将其数据读取到内存中
    // 避免对数据库的频繁访问
    void initChatMembers();
    void startUpdateTimer();
    QMap<ChatMember* ,int>* getFriendsMap(qint64 memberId);
    void addChatMemberUnread(qint64 memberId, qint64 friendId);
    void clearChatMemberUnread(qint64 memberId, qint64 friendId);
    int getChatMemberUnread(qint64 memberId, qint64 friendId);
    ChatMember * getSingleMember(qint64 memberId);
private:
    // 管理所有的ChatMemember
    QHash<qint64, ChatMember*> chatMemberHash;

    QTimer* m_updateTimer = nullptr;
};
