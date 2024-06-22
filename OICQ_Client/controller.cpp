#include "controller.h"

Controller::Controller(QObject *parent)
    : QObject{parent}
{}

Controller &Controller::singleTon()
{
    static Controller controller;
    return controller;
}


FriendModel *Controller::getFriendModel()
{
    return &FriendModel::singleTon();
}


FriendRequestModel *Controller::getFriendRequestModel()
{
    return &FriendRequestModel::singleTon();
}

MessageModel *Controller::getMessageModel(int)
{
    return &MessageModel::singleTon();
}


TcpSocket *Controller::getTcpSocket(){
    return &TcpSocket::singleTon();
}


/// FriendModel中的函数
void Controller::updateMessageModel(int index)
{
    FriendModel::singleTon().updateMessageModel(index);
}

void Controller::sendMessage(QString message, int index, int type)
{
    FriendModel::singleTon().sendMessage(message, index, type);
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

/// FriendRequestModel中的函数
void Controller::choseAddFriend(int res, int index)
{
    FriendRequestModel::singleTon().choseAddFriend(res, index);
}

/// TcpSokcet中的函数
void Controller::connectToServer()
{
    TcpSocket::singleTon().connectToServer();
}

void Controller::packingMessage(QString value,int msgType)
{
    TcpSocket::singleTon().packingMessage(value, msgType);
}

