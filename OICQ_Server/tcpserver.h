#ifndef TCPSERVER_H
#define TCPSERVER_H

// 监听客户端的连接
// 符合is-a的关系,采用继承的方式使用QTcpServer的api
#include <QObject>
#include <QTcpServer>
class ClientHandler;
#include <QJsonValue>
#include <QImage>
class QThread;
class TcpServer : public QTcpServer {
    Q_OBJECT
public:
    explicit TcpServer(QObject* parent = 0);
    ~TcpServer();
    void closeListen();
    void sendToClient(QJsonValue);
    static TcpServer& singleTon();
    void transferFile(qint64 from ,qint64 to, QString filename,QString size,qint64 messageId);
signals:
    void signalUserStatus(const QString& text);

private slots:
    void on_disconnected(ClientHandler *, bool);
    void on_forwardMessages(QJsonValue,qint64,QList<QImage>);
    void on_addFriend(qint64,QString,qint64,QList<QImage>&);
    void on_addFriendRes(QJsonValue jsonvalue,qint64 friendId, QList<QImage>&);

private:

    bool startListen(int port = 1234);

private:
    //管理多个socket
    QList<ClientHandler*> socketList;
    //方便查找对应的socket
    QMap<qint64,ClientHandler*> socketMap;
    // 管理socket对应的thread
    QList<QThread*> threadList;

protected:
    void incomingConnection(qintptr socketDescriptor);

};

#endif // TCPSERVER_H
