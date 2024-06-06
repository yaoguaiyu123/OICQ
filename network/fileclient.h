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
#include <QEventLoop>

class FileClient : public QObject
{
    Q_OBJECT

public:
    FileClient(QObject *parent = nullptr);
    bool connectToServer(const QString &host, quint16 port);
    virtual ~FileClient() override;
public slots:
    void handleBytesWritten(qint64 size);
    // void handleFileBytesWritten(qint64 size);
    qint64 writeByteArray(const QByteArray& byteArray);
    void uploadFile(const QString& filePath, qint64 from ,qint64 to,qint64 messageId);
    void downloadFile(qint64 messageId, qint64 from, qint64 to,  const QString &filepath);
    void readDataFromServer();

signals:
    void complete();
private:
    QTcpSocket *m_socket;
    // 上传文件
    qint64 haveWritten;
    qint64 toWrite;
    // 下载文件
    qint64 haveRead;
    qint64 toRead;
    QEventLoop loop;
    QFile *recvFile;

    QString recvfilename;
    bool is_Dbegin = false;
};

#endif // CLIENT_H
