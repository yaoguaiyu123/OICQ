#include "controller.h"
#include "global.h"

Controller::Controller(QObject *parent)
    : QObject{parent}
{}

Controller &Controller::singleTon()
{
    static Controller controller(nullptr);
    return controller;
}


/// FriendModel
void Controller::updateMessageModel(int index)
{
    if (index == -1) {
        return;
    }
    FriendModel::singleTon().updateMessageModel(index);
}

void Controller::sendMessage(QString message, int index, int type)
{
    if (type == PrivateMessage) {
        int startIndex = message.indexOf("<body") + message.mid(message.indexOf("<body")).indexOf(">") + 1;
        QString cleanMessage = message.mid(startIndex);   //去除开头
        FriendModel::singleTon().sendTextMessage(cleanMessage, index);
    } else {
        FriendModel::singleTon().sendFileMessage(message, index);
    }
}

void Controller::downloadFileRequest(int friendiIndex, int messageIndex, const QString& filepath)
{
    if (friendiIndex < 0 || messageIndex < 0) {
        return;
    }
    FriendModel::singleTon().downloadFileRequest(friendiIndex, messageIndex, filepath);
}

void Controller::updateMyHead(QString strurl)
{
    FriendModel::singleTon().updateMyHead(strurl);
}

void Controller::cancelUploadOrDownload(int friendIndex, int messageIndex)
{
    if (friendIndex < 0 || messageIndex < 0) {
        return;
    }
    FriendModel::singleTon().cancelUploadOrDownload(friendIndex, messageIndex);
}

QList<QString> Controller::currentWindowImages(int friendIndex, int messageIndex)
{
    if (friendIndex < 0 || messageIndex < 0) {
        return QList<QString>();
    }
    return FriendModel::singleTon().currentWindowImages(friendIndex, messageIndex);
}

/// FriendRequestModel
void Controller::choseAddFriend(int res, int index)
{
    if (index < 0) {
        return;
    }
    const QJsonValue sendobj = FriendRequestModel::singleTon().choseAddFriend(res, index);
    TcpSocket::singleTon().packingMessageFromJson(sendobj, AddFriendRes);

}

/// TcpSokcet
void Controller::connectToServer()
{
    TcpSocket::singleTon().connectToServer();
}

void Controller::loginToServer(qint64 accountId,QString password, int msgType)
{
    QJsonObject sendObj;
    sendObj.insert("userId", accountId);
    sendObj.insert("password", password);
    TcpSocket::singleTon().packingMessageFromJson(sendObj, msgType);
}

void Controller::sendAddFriendRequest(qint64 friendId, int msgType)
{
    QJsonObject sendObj;
    sendObj.insert("friendId", friendId);
    TcpSocket::singleTon().packingMessageFromJson(sendObj, msgType);
}

