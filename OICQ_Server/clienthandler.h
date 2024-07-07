#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QJsonValue>
#include <QImage>
#include "global.h"
#include "chatmember.h"

struct MESG;

// 消息socket
//负责维护一个客户端的通信
class QTcpSocket;
class QJsonObject;
class ClientHandler : public QObject {
    Q_OBJECT
public:
    explicit ClientHandler(qintptr socketDescriptor,QObject* parent = nullptr);
    qint64 userId();
    void sendToClient(MESG *);
    void packingMessage(QJsonValue jsonvalue, int msgType, QList<QImage> imageList = QList<QImage>());
    void returnLoginRes(int resType);
public slots:
    void init();
private slots:
    void on_ready_read();
signals:
    void disconnected(ClientHandler* , bool);
    void forwardMessages(QJsonValue,qint64,QList<QImage>);
    void addFriend(qint64, QString, qint64, QList<QImage>&);
    void addFriendRes(QJsonValue jsonvalue,qint64 friendId, QList<QImage>&);
    void loginRequest(qint64 userId,ClientHandler* clientHandler);
private:
    qintptr m_descriptor;
    QTcpSocket* m_tcpSocket;
    qint64 m_userId = -1;
    uchar* m_sendbuf;
    RecvBuf m_recvbuf;
    QImage m_headImage; //记录当前的头像
    bool m_isLogin = false;
private:
    void parsePackage();
    void parsePrivateMessage(QJsonValue, QList<QImage> images);
    void parseLogin(QJsonValue);
    void parseAddFriend(QJsonValue jsonvalue);
    void parseUpdateHead(qint64 userId,  QList<QImage> images);
    void parseAddFriendRes(QJsonValue jsonvalue);
    void parseUpdateIsRead(QJsonValue jsonvalue);

    void sendFriendList();
    void sendFriendRequestList();
    void sendMessageList();
};

#endif // CLIENTHANDLER_H
