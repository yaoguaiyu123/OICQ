#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QTcpSocket>
#include <QFile>
#include <QDir>

//传输文件的网络套接字

class FileClientHandler : public QObject
{
public:
    FileClientHandler();
    virtual ~FileClientHandler() override;
    void parseDownloadFile(const QString& filename);
public slots:
    void initSocket(int socketDescriptor);
    void handleBytesWritten(qint64 size);
private slots:
    void onReadyRead();
private:
    QTcpSocket * m_socket = nullptr;
    bool m_inited = false;
    // 接收
    bool is_Ubegin = false;
    qint64 recvSize = 0, totalSize = 0, from, to, messageId;
    QString filename, newFileName;
    QFile file;
    QDir dir;
    // 发送
    bool is_Dbegin = false;
    qint64 haveWritten;
    qint64 toWrite;
};

#endif // CLIENTHANDLER_H
