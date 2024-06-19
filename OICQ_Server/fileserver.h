#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
// 文件服务器
// 大致思路是一个请求 一个socket一个线程

class FileServer : public QTcpServer
{
    Q_OBJECT

public:
    FileServer(QObject *parent = nullptr);
    ~FileServer();
    bool startFileServer(const QHostAddress &address, quint16 port);
protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
};

#endif // SERVER_H
