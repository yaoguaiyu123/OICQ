#include "chatmemberdata.h"
#include "database/dbmanager.h"

ChatMemberData::ChatMemberData(QObject *parent)
    : QObject{parent}
{
    initChatMembers();
}


ChatMemberData& ChatMemberData::singleTon()
{
    static ChatMemberData singleTon;
    return singleTon;
}

void ChatMemberData::addChatMemberUnread(qint64 memberId, qint64 friendId)
{
    ChatMember* chatMember = chatMemberHash.value(memberId);
    ChatMember* friendMember = chatMemberHash.value(friendId);
    chatMember->addUnread(friendMember);
}

void ChatMemberData::clearChatMemberUnread(qint64 memberId, qint64 friendId)
{
    ChatMember* chatMember = chatMemberHash.value(memberId);
    ChatMember* friendMember = chatMemberHash.value(friendId);
    chatMember->clearUnread(friendMember);
}

int ChatMemberData::getChatMemberUnread(qint64 memberId, qint64 friendId)
{
    ChatMember* chatMember = chatMemberHash.value(memberId);
    ChatMember* friendMember = chatMemberHash.value(friendId);
    return chatMember->getUnread(friendMember);
}


// 好友表作为经常需要访问的表将其数据读取到内存中
// 避免对数据库的频繁访问
void ChatMemberData::initChatMembers(){
    QList<QVariantMap> listMember = DBManager::singleTon().queryDataUsers();
    // 初始化所有用户
    for (QVariantMap varmap : listMember) {
        qint64 accountId = varmap.value("accountid").toLongLong();
        QString username = varmap.value("username").toString();
        QString headpath = varmap.value("headpath").toString();
        ChatMember* member = new ChatMember(accountId, headpath, username);
        chatMemberHash.insert(accountId, member);
    }
    // 初始化所有用户的好友
    for (QVariantMap varmap : listMember) {
        qint64 accountId = varmap.value("accountid").toLongLong();
        ChatMember* chatMember = chatMemberHash.value(accountId);
        QList<QVariantMap> listFriends = DBManager::singleTon().queryDataFriends(accountId);
        for (QVariantMap friendMap : listFriends) {
            qint64 friendid = friendMap.value("friendid").toLongLong();
            qint64 unreadCount = friendMap.value("unreadCount").toInt();
            ChatMember* friendMember = chatMemberHash.value(friendid);
            chatMember->addFriend(friendMember, unreadCount);
        }
    }
}
