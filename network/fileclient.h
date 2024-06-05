#ifndef CLIENT_H
#define CLIENT_H

// 文件传输的套接字
// 注册为一个单例类

// 开发日记
// 1. 在子thread中创建socket导致发送失败
// 2. 打算使用信号槽和 moveToThread实现

#include <QTcpSocket>
#include <QObject>
#include <QFile>

class FileClient : public QObject
{
    Q_OBJECT

public:
    FileClient(QObject *parent = nullptr);
    bool connectToServer(const QString &host, quint16 port);
    void handleBytesWritten(qint64 size);
    void readDataFromServer(const QString &filepath);
    ~FileClient();
public slots:
    qint64 writeByteArray(const QByteArray& byteArray);
    void uploadFile(const QString& filePath, qint64 from ,qint64 to,qint64 messageId);
    void downloadFile(qint64 messageId, qint64 from, qint64 to,  const QString &filepath);
signals:
    void complete();
private:
    // QTcpSocket *m_socket;
    QTcpSocket *socket = nullptr;
    // 上传文件
    qint64 haveWritten;
    qint64 toWrite;
    // 下载文件
    qint64 haveRead;
    qint64 toRead;

    QString recvfilename;
    bool is_Dbegin = false;
};

#endif // CLIENT_H
