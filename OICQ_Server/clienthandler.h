#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QJsonValue>
#include <QImage>

struct MESG;

struct RecvBuf {
    uchar* buffer;
    qint32 beginIndex;
    qint32 endIndex;
    qint32 capacity;

    RecvBuf(int cap = 1024 * 1024 * 16)
        : capacity(cap), beginIndex(0), endIndex(0)
    {
        buffer = new uchar[capacity];
    }

    ~RecvBuf()
    {
        delete[] buffer;
    }

    // 可用空间
    qint32 availableSpaceSize() {
        if (endIndex >= beginIndex) {
            return capacity - (endIndex - beginIndex);
        } else {
            return beginIndex - endIndex;
        }
    }

    // 当前缓冲区内有效数据的长度
    qint32 dataSize() {
        if (endIndex >= beginIndex) {
            return endIndex - beginIndex;
        } else {
            return capacity - beginIndex + endIndex;
        }
    }

    //拷贝数据到目标区域
    void copyDataToDest(uchar* dest, qint32 numBytes) {
        if (numBytes > dataSize()) {
            qDebug() << "拷贝请求过多的数据";
            return;
        }
        qint32 firstPartLength = qMin(numBytes, capacity - beginIndex);
        memcpy(dest, buffer + beginIndex, firstPartLength);

        if (numBytes > firstPartLength) {
            memcpy(dest + firstPartLength, buffer, numBytes - firstPartLength);
        }
    }

    // 增加数据的函数
    void append(const uchar* data, qint32 len)
    {
        if (len <= 0) {
            return;
        }
        qint32 availableSpace = availableSpaceSize();
        if (len > availableSpace) {
            qDebug() << "警告: 缓冲区内存已满!!!";
            return;
        }

        if (endIndex + len < capacity) {
            // 直接添加
            memcpy(buffer + endIndex, data, len);
            endIndex += len;
        } else {
            // 需要回绕
            qint32 part1Len = capacity - endIndex;
            memcpy(buffer + endIndex, data, part1Len);
            qint32 part2Len = len - part1Len;
            memcpy(buffer, data + part1Len, part2Len);
            endIndex = part2Len;
        }

        if (endIndex == capacity) {
            endIndex = 0;
        }
    }
};

// 消息socket
//负责维护一个客户端的通信
class QTcpSocket;
class QJsonObject;
class ClientHandler : public QObject {
    Q_OBJECT
public:
    explicit ClientHandler(qintptr socketDescriptor,QObject* parent = nullptr);
    qint64 userId();
    void sendToClient(MESG *);
    void packingMessage(QJsonValue jsonvalue, int msgType, QList<QImage> imageList = QList<QImage>());
    void returnLoginRes(int resType);
public slots:
    void init();
private slots:
    void on_ready_read();
signals:
    void disconnected(int);
    void forwardMessages(QJsonValue,qint64,QList<QImage>);
    void addFriend(qint64, QString, qint64, QList<QImage>&);
    void addFriendRes(QJsonValue jsonvalue,qint64 friendId, QList<QImage>&);
    void loginRequest(qint64 userId,ClientHandler* clientHandler);
private:
    qintptr m_descriptor;
    QTcpSocket* m_tcpSocket;
    qint64 m_userId = -1;
    uchar* m_sendbuf;
    RecvBuf m_recvbuf;
    QImage m_headImage;  //记录当前的头像
private:
    void parsePackage();
    void parsePrivateMessage(QJsonValue, QList<QImage> images);
    void parseLogin(QJsonValue);
    void parseAddFriend(QJsonValue jsonvalue);
    void parseUpdateHead(qint64 userId,  QList<QImage> images);
    void parseAddFriendRes(QJsonValue jsonvalue);
    void parseUpdateIsRead(QJsonValue jsonvalue);

    void sendFriendList();
    void sendFriendRequestList();
    void sendMessageList();
};

#endif // CLIENTHANDLER_H
