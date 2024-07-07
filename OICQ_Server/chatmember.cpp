#include "chatmember.h"

ChatMember::ChatMember(qint64 accountId, QString headpath,QString memberName, QObject *parent)
    :m_accountId(accountId),
    m_headpath(headpath),
    m_memberName(memberName),
    QObject(parent)
{
    ;
}

void ChatMember::addFriend(ChatMember* newFriend, int unreadCount) {
    if (!friends.contains(newFriend)) {
        friends.insert(newFriend, unreadCount);
    }
}

bool ChatMember::removeFriend(ChatMember* exFriend) {
    if (friends.remove(exFriend)) {
        return true;
    }
    return false;
}

QList<ChatMember*> ChatMember::getFriends() const {
    return friends.keys();
}

int ChatMember::getUnreadCount(ChatMember* friendMember) const {
    return friends.value(friendMember, 0);  // 如果好友不存在，返回0
}

void ChatMember::setUnreadCount(ChatMember* friendMember, int count) {
    if (friends.contains(friendMember)) {
        friends[friendMember] = count;
    }
}

//添加未读的消息条数
void ChatMember::addUnread(ChatMember* friend_)
{
    friends[friend_] = friends.value(friend_) + 1;
}

//清空未读的消息条数
void ChatMember::clearUnread(ChatMember* friend_)
{
    friends[friend_] = 0;
}

//获取未读消息条数
int ChatMember::getUnread(ChatMember* friend_)
{
    return friends[friend_];
}
