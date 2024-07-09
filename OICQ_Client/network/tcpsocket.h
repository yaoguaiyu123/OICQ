#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include "../global.h"
#include <QJsonValue>
#include <QImage>
#include <QQmlEngine>
#include <QJSEngine>
#include <QtQml/qqmlregistration.h>


class QQmlEngine;
class QJSEngine;
struct MESG;

class TcpSocket : public QTcpSocket {
    Q_OBJECT
    QML_SINGLETON   //注册为单例对象
    QML_NAMED_ELEMENT(TcpSocket)
public:
    explicit TcpSocket(QObject* parent);
    static TcpSocket& singleTon();

    static TcpSocket* create(QQmlEngine* qmlengine, QJSEngine*) {
        TcpSocket* object = &singleTon();
        object->m_jsvalue = qmlengine->newSymbol(QString("TcpSocket"));
        return object;
    }

    void connectToServer();
    void sendMessage(MESG* send);
    int getUserId();
    void packingMessageFromJson(const QJsonValue &value, int msgType, QList<QImage> imageList = QList<QImage>());private slots:
    void on_readyRead();
private:
    QHostAddress* m_hostAddress = nullptr;
    qint64 m_userId = 0; // qq号,用于标识每一个用户
    uchar* m_sendbuf = nullptr; // uchar处理二进制数据
    RecvBuf m_recvbuf;
    QJSValue m_jsvalue;
signals:
    void loginReturn(const QImage & image);
    void loginReturnToQml(int);
    void friendListReturn(const QJsonValue&,const QList<QImage>&);
    void privateMessageReturn(const QJsonValue&,const QList<QImage>& images);
    void updateHeadReturn(const QImage &);
    void addFriend(int);
    void addFriendRequest(const QJsonValue& jsonvalue,const QList<QImage> & images);
    void addFriendSingleRequest(const QJsonValue& jsonvalue,const QList<QImage> & images);
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
