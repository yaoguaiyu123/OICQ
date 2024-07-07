#include "chatmember.h"

// 类的实现部分
ChatMember::ChatMember(QObject *parent)
    : QObject(parent)
{
}

void ChatMember::addFriend(ChatMember* newFriend) {
    if (!friends.contains(newFriend)) {
        friends.append(newFriend);
        emit friendAdded(newFriend);
    }
}

bool ChatMember::removeFriend(ChatMember* exFriend) {
    if (friends.removeOne(exFriend)) {
        emit friendRemoved(exFriend);
        return true;
    }
    return false;
}

QList<ChatMember*> ChatMember::getFriends() const {
    return friends;
}


