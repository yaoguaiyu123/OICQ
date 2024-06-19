#include "fileserver.h"
#include "fileclienthandler.h"
#include <QDebug>
#include <QTcpSocket>
#include <QThread>

FileServer::FileServer(QObject* parent)
    : QTcpServer(parent)
{
    startFileServer(QHostAddress::Any, 8081);
}

FileServer::~FileServer()
{
}

bool FileServer::startFileServer(const QHostAddress& address, quint16 port)
{
    qDebug() << tr("文件服务器开始在%1端口监听").arg(port);
    return listen(address, port);
}

// 接收
void FileServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "接收到新的文件上传或下载请求";

    FileClientHandler *client = new FileClientHandler();
    QThread *thread = new QThread();
    client->moveToThread(thread);

    connect(thread, &QThread::started, client, [client, socketDescriptor]() {
        client->initSocket(socketDescriptor);
    });
    connect(client, &FileClientHandler::destroyed, thread, &QThread::quit); //线程停止
    connect(client, &FileClientHandler::destroyed, [client]() {
        qDebug() << "接收到信号断开，处理socket和thread资源回收";
    });

    connect(thread, &QThread::finished, thread, &QThread::deleteLater);  //释放线程资源
    thread->start();
}
