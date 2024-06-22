#pragma once

#include <QObject>
#include "messagemodel.h"
#include "friendmodel.h"
#include "friendrequestmodel.h"
#include "network/tcpsocket.h"
#include <QtQml/qqmlregistration.h>
// 控制器层 ,用于为ui层提供插槽和接口

class Controller : public QObject
{
    Q_OBJECT
    QML_SINGLETON   //注册为单例对象
    QML_NAMED_ELEMENT(Controller)
public:
    explicit Controller(QObject *parent = nullptr);
    static Controller &singleTon();
    Q_INVOKABLE FriendModel *getFriendModel();
    Q_INVOKABLE FriendRequestModel *getFriendRequestModel();
    Q_INVOKABLE MessageModel *getMessageModel(int);
    Q_INVOKABLE TcpSocket *getTcpSocket();

    // friendModel中的函数
    Q_INVOKABLE void updateMessageModel(int index);
    Q_INVOKABLE void sendMessage(QString,int,int);
    Q_INVOKABLE void downloadFileRequest(int friendiIndex, int messageIndex,const QString& filepath);
    Q_INVOKABLE void updateMyHead(QString);
    Q_INVOKABLE void cancelUploadOrDownload(int friendIndex,int messageIndex);
    Q_INVOKABLE QList<QString> currentWindowImages(int friendIndex, int messageIndex);

    // friendRequestModel中的函数
    Q_INVOKABLE void choseAddFriend(int, int);

    // tcpSocket中的函数
    Q_INVOKABLE void connectToServer();
    Q_INVOKABLE void packingMessage(QString value, int msgType);
signals:
};
