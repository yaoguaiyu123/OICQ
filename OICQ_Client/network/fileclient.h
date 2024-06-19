#ifndef CLIENT_H
#define CLIENT_H

// 文件传输的套接字
// 注册为一个单例类


#include <QTcpSocket>
#include <QObject>
#include <QFile>
#include <QEventLoop>
#include "global.h"
class QTimer;

class FileClient : public QObject
{
    Q_OBJECT

public:
    FileClient(int friendIndex ,int messageIndex, QObject *parent = nullptr);
    bool connectToServer(const QString &host, quint16 port);
    virtual ~FileClient() override;
    int getIndex();
    int getMessageIndex();
    void cancelTransfer();
public slots:
    void handleBytesWritten(qint64 size);
    // void handleFileBytesWritten(qint64 size);
    qint64 writeByteArray(const QByteArray& byteArray);
    void uploadFile(const QString& filePath, qint64 from ,qint64 to,qint64 messageId);
    void downloadFile(qint64 messageId, qint64 from, qint64 to,  const QString &filepath);
    void readDataFromServer();
private slots:
    void handlerTimeout();
signals:
    void complete();
    void updateFileMessage(int friendIndex,int messageIndex,qint64 have, qint64 total);
    void aboutToCancel();
private:
    bool m_used = false;
    QTcpSocket *m_socket;
    QTimer *m_timer;
    int m_index = -1;  // 标记是哪个窗口的消息
    int m_messageIndex = -1;  // 标记是窗口的哪条消息
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
    bool upOrDown;
};

#endif // CLIENT_H
