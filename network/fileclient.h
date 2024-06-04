#ifndef CLIENT_H
#define CLIENT_H

// 文件传输的套接字

#include <QTcpSocket>
#include <QObject>
#include <QFile>

class FileClient : public QObject
{
    Q_OBJECT

public:
    FileClient(QObject *parent = nullptr);
    bool connectToServer(const QString &host, quint16 port);
    void uploadFile(const QString& filePath);
    void handleBytesWritten(qint64 size);
    ~FileClient();
public slots:
    qint64 writeByteArray(const QByteArray& byteArray);
signals:
private:
    QTcpSocket socket;
    QFile file;
    qint64 haveWritten;
    qint64 toWrite;
};

#endif // CLIENT_H
