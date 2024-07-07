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
    explicit Controller(QObject *parent);
    static Controller &singleTon();

    static Controller* create(QQmlEngine* qmlengine, QJSEngine*) {
        Controller* object = &singleTon();
        object->m_jsvalue = qmlengine->newSymbol(QString("Controller"));
        return object;
    }

    Q_INVOKABLE FriendModel *getFriendModel();
    Q_INVOKABLE FriendRequestModel *getFriendRequestModel();
    Q_INVOKABLE MessageModel *getMessageModel(int);
    Q_INVOKABLE TcpSocket *getTcpSocket();

    // friendModel
    Q_INVOKABLE void updateMessageModel(int index);
    Q_INVOKABLE void sendMessage(QString,int,int);
    Q_INVOKABLE void downloadFileRequest(int friendiIndex, int messageIndex,const QString& filepath);
    Q_INVOKABLE void updateMyHead(QString);
    Q_INVOKABLE void cancelUploadOrDownload(int friendIndex,int messageIndex);
    Q_INVOKABLE QList<QString> currentWindowImages(int friendIndex, int messageIndex);

    // friendRequestModel
    Q_INVOKABLE void choseAddFriend(int, int);

    // tcpSocket
    Q_INVOKABLE void connectToServer();
    Q_INVOKABLE void loginToServer(qint64 accountId,QString password, int msgType);
    Q_INVOKABLE void sendAddFriendRequest(QString friendId, int msgType);
signals:
private:
    QJSValue m_jsvalue;
};
