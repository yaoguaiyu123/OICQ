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
        FriendModel::singleTon().sendTextMessage(message, index);
    } else {
        FriendModel::singleTon().sendFileMessage(message, index);
    }
}

void Controller::downloadFileRequest(int friendiIndex, int messageIndex, const QString& filepath)
{
    FriendModel::singleTon().downloadFileRequest(friendiIndex, messageIndex, filepath);
}

void Controller::updateMyHead(QString strurl)
{
    FriendModel::singleTon().updateMyHead(strurl);
}

void Controller::cancelUploadOrDownload(int friendIndex, int messageIndex)
{
    FriendModel::singleTon().cancelUploadOrDownload(friendIndex, messageIndex);
}

QList<QString> Controller::currentWindowImages(int friendIndex, int messageIndex)
{
    return FriendModel::singleTon().currentWindowImages(friendIndex, messageIndex);
}

/// FriendRequestModel
void Controller::choseAddFriend(int res, int index)
{
    FriendRequestModel::singleTon().choseAddFriend(res, index);
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

