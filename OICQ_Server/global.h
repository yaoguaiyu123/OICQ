#ifndef GLOBAL_H
#define GLOBAL_H
#include <QList>
#include <QString>
#include <QMap>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QDebug>
#include <QList>
#include <QByteArray>
#include <QRandomGenerator>

QString getRandomString(int nLen);



QJsonValue byteArrayToJson(const QByteArray& jsonData);


struct Recode {
    qint64 id;   //消息ID
    QString date;
    QString type;
    QString message;
    QString filename;
    QString filesize;

    Recode(const QString &date, const QString &type, const QString &message)
        : date(date), type(type), message(message) {
    }

    Recode(const QString &date, const QString &type, const QString &message,
        const QString &filename, const QString &filesize)
        : date(date), type(type), message(message), filename(filename), filesize(filesize) {
    }
};

struct Friend {
    QString name;
    QString headPath;
    qint64 userid; // QQ号
    int unreadMessageNum;
};

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

enum MSG_TYPE {
    PrivateMessage,
    FileMessage,
    AddFriend,
    Login,
    FriendList,
    UpdateHead,
    AddFriendRequest,
    AddFriendRes,
    FriendRequestList,
    UpLoadFileRequest,
    MessageList,
    UpdateIsRead
};

enum RETURN{
    Repeat,
    Success,
    Fail,
    Repeat2
};

struct MESG //消息结构体
{
    MSG_TYPE msg_type;
    uchar* data;
    int len;
    QList<QByteArray> imagesData;
};
Q_DECLARE_METATYPE(MESG *);

#ifndef MB
#define MB 1024*1024
#endif

#ifndef KB
#define KB 1024
#endif


#endif // GLOBAL_H
