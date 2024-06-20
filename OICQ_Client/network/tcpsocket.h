#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
class QQmlEngine;
class QJSEngine;
struct MESG;
#include <QJsonValue>
#include <QImage>

// 可以循环使用的缓冲区
struct RecvBuf {
    uchar* buffer;
    qint32 beginIndex;
    qint32 endIndex;
    qint32 capacity;

    RecvBuf(int cap = 1024 * 1024 * 32)
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


class TcpSocket : public QTcpSocket {
    Q_OBJECT
public:
    explicit TcpSocket(QObject* parent = nullptr);
    Q_INVOKABLE void connectToServer();
    void sendMessage(MESG* send);
    static TcpSocket& singleTon();
    int getUserId();
    Q_INVOKABLE void packingMessage(QString value, int msgType, QList<QImage>imageList = QList<QImage>());
    void packingMessageFromJson(QJsonValue value, int msgType, QList<QImage>imageList = QList<QImage>());
private slots:
    void on_ready_read();
private:
    QHostAddress* m_hostAddress;
    qint64 m_userId = 0; // qq号,用于标识每一个用户
    uchar* m_sendbuf; // uchar处理二进制数据
    RecvBuf m_recvbuf;
signals:
    // TODO 待实现的逻辑
    void sendDataOver(bool); // 发送数据结束的信号
    void loginReturn(QImage &);
    void loginReturnToQml(int);
    void friendListReturn(QJsonValue&,QList<QImage>&);
    void privateMessageReturn(QJsonValue&,QList<QImage>& images);
    void updateHeadReturn(const QImage &);
    void addFriend(int);
    void addFriendRequest(QString,qint64,QList<QImage> & images);
    void addFriendRes(QJsonValue jsonvalue, QList<QImage>&);
    void fileMessage(QJsonValue &jsonvalue);
    void historyMessageList(QJsonValue& jsonvalue);
private:
    void parsePackage();  //处理包的函数
    void parseLogin(QJsonValue& jsonvalue,QList<QImage> &images);
    void parseFriendList(QJsonValue& jsonvalue,QList<QImage>& images);
    void parsePrivateMessage(QJsonValue& jsonvalue,QList<QImage> &images);
    void parseUpdateHead(QList<QImage>& images);
    void parseAddFriend(QJsonValue& jsonvalue);
    void parseAddFriendRequest(QJsonValue& jsonvalue,QList<QImage> &);
    void parseAddFriendRes(QJsonValue& jsonvalue, QList<QImage>&);
    void parseFriendRequestList(QJsonValue& jsonvalue, QList<QImage>&);
    void perseFileMessage(QJsonValue& jsonvalue);
    void parseHistoryMessageList(QJsonValue& jsonvalue);

};

#endif // TCPSOCKET_H
