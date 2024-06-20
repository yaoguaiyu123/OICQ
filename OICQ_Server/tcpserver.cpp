#include "tcpserver.h"
#include <QTcpServer>
#include <QDebug>
#include "clienthandler.h"
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtEndian>
#include "global.h"
#include <QThread>
#include "database/dbmanager.h"
#include <QDateTime>

TcpServer::TcpServer(QObject* parent)
    : QTcpServer { parent }
{
    qDebug() << tr("消息服务器开始在%1端口监听").arg(8080);
    startListen(8080);  //在8080端口进行监听
}

TcpServer& TcpServer::singleTon()
{
    static TcpServer server;
    return server;
}

TcpServer::~TcpServer()
{
    closeListen();
}

bool TcpServer::startListen(int port)
{
    closeListen();
    // 开始在port端口进行监听
    return listen(QHostAddress::Any, port);
}

void TcpServer::closeListen()
{
    if (isListening()) {
        close();
    }
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "服务端获取一个新的连接,正在创建套接字.....";
    ClientHandler* clientHandler = new ClientHandler(socketDescriptor, this);
    QThread* thread = new QThread;
    connect(thread, &QThread::started, clientHandler, &ClientHandler::init);
    connect(clientHandler,
            &ClientHandler::loginRequest,
            [this](qint64 userid, ClientHandler* clientHandler) {
                qDebug() << "server接受到clientHandler的登录判断信号";
                if (socketMap.value(userid) == nullptr) {
                    if (clientHandler == nullptr) {
                        return;
                    }
                    socketMap.insert(userid, clientHandler); //加入进行管理
                    clientHandler->returnLoginRes(Success);
                } else {
                    clientHandler->returnLoginRes(Repeat);
                }
    });
    connect(clientHandler, &ClientHandler::disconnected, this, &TcpServer::on_disconnected);
    connect(clientHandler, &ClientHandler::forwardMessages, this, &TcpServer::on_forwardMessages);
    connect(clientHandler, &ClientHandler::addFriend, this, &TcpServer::on_addFriend);
    connect(clientHandler, &ClientHandler::addFriendRes, this, &TcpServer::on_addFriendRes);
    clientHandler->moveToThread(thread);   //在一个线程中运行
    thread->start();
    threadList.append(thread);
    socketList.append(clientHandler);
}

// 断开连接
void TcpServer::on_disconnected(ClientHandler* handler, bool isLogined)
{
    if (isLogined) {
        socketMap.remove(handler->userId());
        qDebug() << QString("用户%1下线").arg(handler->userId());
    }
    for (int i = 0; i < socketList.length(); ++i) {
        if (socketList[i] == handler) {
            delete socketList[i];
            socketList.remove(i);
            // 将线程对象也释放
            threadList[i]->quit();
            threadList[i]->wait();
            delete threadList[i];
            threadList.remove(i);
        }
    }
}


// 进行消息转发的槽函数
// 根据from to发送消息
void TcpServer::on_forwardMessages(QJsonValue jsonvalue,qint64 from,QList<QImage> images)
{
    if (!jsonvalue.isObject()) {
        return;
    }
    QJsonObject object = jsonvalue.toObject();
    qDebug() << "server接收到转发消息:" + object.value("message").toString();
    qint64 to = object.value("to").toInteger();
    QString msg = object.value("message").toString();
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    // 加入到messages数据库
    DBManager::singleTon().insertNormalMessage(from, to, msg, strDateTime, "normal");

    // TODO 优化为Map查找 添加好友判断
    for (auto& userSocket : socketList) {
        if (userSocket->userId() == to) {
            QJsonObject sendObj;
            sendObj.insert("from", from);
            sendObj.insert("message", msg);
            userSocket->packingMessage(sendObj, PrivateMessage, images);
            break;
        }
    }
}

// 转发好友添加
void TcpServer::on_addFriend(qint64 to, QString fromname, qint64 from, QList<QImage> &imagelist)
{
    qDebug() << "接收到添加好友的信息 " << from << " to " << to;
    for (auto& userSocket : socketList) {
        if (userSocket->userId() == to) {
            QJsonObject sendObj;
            sendObj.insert("fromname", fromname);
            sendObj.insert("fromid", from);
            userSocket->packingMessage(sendObj, AddFriendRequest,imagelist);
            break;
        }
    }
}

//处理好友添加的结果
void TcpServer::on_addFriendRes(QJsonValue jsonvalue,qint64 friendId, QList<QImage>& images){
    for (auto& userSocket : socketList) {
        if (userSocket->userId() == friendId) {
            userSocket->packingMessage(jsonvalue, AddFriendRes,images);
            break;
        }
    }
}

// 在消息服务器进行文件消息的转发
void TcpServer::transferFile(qint64 from, qint64 to, QString filename, QString filesize,qint64 messageId)
{
    // 需要在数据库中存放文件的映射
    DBManager::singleTon().insertIntoFiles(messageId, from, to, filename);
    QFileInfo info(filename);
    QString fname = info.fileName().mid(8);
    QString msg = "[文件] " + fname;
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"); // 在消息数据库中存放文件消息

    // 在数据库中存放文件消息
    DBManager::singleTon().insertMessage(from, to, msg, messageId, fname, filesize, strDateTime, "file");

    // 转发文件消息
    for (auto& userSocket : socketList) {
        if (userSocket->userId() == to) {
            QJsonObject sendObj;
            sendObj.insert("from", from);
            sendObj.insert("filename", fname);
            sendObj.insert("messageId", messageId);
            sendObj.insert("filesize", filesize);
            userSocket->packingMessage(sendObj, FileMessage);
            break;
        }
    }
}



