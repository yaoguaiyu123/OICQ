#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
class QQmlEngine;
class QJSEngine;
struct MESG;
#include <QJsonValue>
#include <QImage>

class TcpSocket : public QTcpSocket {
    Q_OBJECT
public:
    explicit TcpSocket(QObject* parent = nullptr);
    Q_INVOKABLE void connectToServer();
    void sendMessage(MESG* send);
    static TcpSocket& singleTon();
    int getUserId();
    Q_INVOKABLE void packingMessage(QString value, int msgType, QList<QImage>imageList = QList<QImage>());
    void packingMessageFromJson(QJsonValue value, int msgType, QList<QImage>imageList = QList<QImage>());
private slots:
    void on_ready_read();
private:
    QHostAddress* m_hostAddress;
    qint64 m_userId = 0; // qq号,用于标识每一个用户
    uchar* m_sendbuf; // uchar处理二进制数据
    QByteArray m_recvbuf;
signals:
    // TODO 待实现的逻辑
    void sendDataOver(bool); // 发送数据结束的信号
    void loginReturn(int, QImage &);
    void loginReturnToQml(int);
    void friendListReturn(QJsonValue&,QList<QImage>&);
    void privateMessageReturn(QJsonValue&,QList<QImage>& images);
    void updateHeadReturn(const QImage &);
    void addFriend(int);
    void addFriendRequest(QString,qint64,QList<QImage> & images);
    void addFriendRes(QJsonValue jsonvalue, QList<QImage>&);
private:
    void parseLogin(QJsonValue jsonvalue,QList<QImage> &images);
    void parseFriendList(QJsonValue jsonvalue,QList<QImage>& images);
    void parsePrivateMessage(QJsonValue jsonvalue,QList<QImage> &images);
    void parseUpdateHead(QList<QImage>& images);
    void parseAddFriend(QJsonValue jsonvalue);
    void parseAddFriendRequest(QJsonValue jsonvalue,QList<QImage> &);
    void parseAddFriendRes(QJsonValue jsonvalue, QList<QImage>&);
    void parseFriendRequestList(QJsonValue jsonvalue, QList<QImage>&);

};

#endif // TCPSOCKET_H
