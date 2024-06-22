#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include "global.h"
#include <QJsonValue>
#include <QImage>

class QQmlEngine;
class QJSEngine;
struct MESG;

class TcpSocket : public QTcpSocket {
    Q_OBJECT
public:
    explicit TcpSocket(QObject* parent = nullptr);
    Q_INVOKABLE void connectToServer();
    void sendMessage(MESG* send);
    static TcpSocket& singleTon();
    int getUserId();
    Q_INVOKABLE void packingMessage(QString value, int msgType, QList<QImage>imageList = QList<QImage>());
    void packingMessageFromJson(const QJsonValue &value, int msgType, QList<QImage> imageList = QList<QImage>());private slots:
    void on_ready_read();
private:
    QHostAddress* m_hostAddress;
    qint64 m_userId = 0; // qq号,用于标识每一个用户
    uchar* m_sendbuf; // uchar处理二进制数据
    RecvBuf m_recvbuf;
signals:
    // TODO 待实现的逻辑
    void sendDataOver(bool); // 发送数据结束的信号
    void loginReturn(const QImage & image);
    void loginReturnToQml(int);
    void friendListReturn(const QJsonValue&,const QList<QImage>&);
    void privateMessageReturn(const QJsonValue&,const QList<QImage>& images);
    void updateHeadReturn(const QImage &);
    void addFriend(int);
    void addFriendRequest(QString,qint64,const QList<QImage> & images);
    void addFriendRes(const QJsonValue &jsonvalue, const QList<QImage>&);
    void fileMessage(const QJsonValue &jsonvalue);
    void historyMessageList(const QJsonValue& jsonvalue);
private:
    void parsePackage();  //处理包的函数
    void parseLogin(const QJsonValue& jsonvalue,const QList<QImage> &images);
    void parseFriendList(const QJsonValue& jsonvalue,const QList<QImage>& images);
    void parsePrivateMessage(const QJsonValue& jsonvalue,const QList<QImage> &images);
    void parseUpdateHead(const QList<QImage>& images);
    void parseAddFriend(const QJsonValue& jsonvalue);
    void parseAddFriendRequest(const QJsonValue& jsonvalue,const QList<QImage> &);
    void parseAddFriendRes(const QJsonValue& jsonvalue, const QList<QImage>&);
    void parseFriendRequestList(const QJsonValue& jsonvalue,const QList<QImage>&);
    void perseFileMessage(const QJsonValue& jsonvalue);
    void parseHistoryMessageList(const QJsonValue& jsonvalue);

};

#endif // TCPSOCKET_H
