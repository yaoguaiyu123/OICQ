#ifndef GLOBAL_H
#define GLOBAL_H
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QList>
#include <QByteArray>

#define IPADDRESS "10.253.14.51"

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

#include <QString>

struct Recode {
    qint64 id;   //消息ID
    QString date;
    QString type;
    QString message;
    QString filename = "";
    QString filesize = "";

    qint64 fileTotalSize = 1;
    qint64 haveRecvOrSendSize = 0;

    Recode(qint64 id,const QString &date, const QString &type, const QString &message)
        :id(id), date(date), type(type), message(message) {
    }


    Recode(qint64 id, const QString &date, const QString &type, const QString &message,
        const QString &filename, const QString &filesize)
        : id(id) , date(date), type(type), message(message), filename(filename), filesize(filesize) {
    }

    Recode(qint64 id,
           const QString &date,
           const QString &type,
           const QString &message,
           const QString &filename,
           const QString &filesize,
           qint64 fileTotalSize,
           qint64 haveRecvOrSendSize)
        : id(id)
        , date(date)
        , type(type)
        , message(message)
        , filename(filename)
        , filesize(filesize)
        , fileTotalSize(fileTotalSize)
        , haveRecvOrSendSize(haveRecvOrSendSize)
    {
    }
};

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


struct Friend {
    QString name;
    QString headPath;
    qint64 userid; // QQ号
    int unreadMessageNum = 0;
};

struct MESG //消息结构体
{
    MSG_TYPE msg_type;
    uchar* data;
    int len;
    QList<QByteArray> imagesData;
};
Q_DECLARE_METATYPE(MESG *);

QString generateRandomAlphanumeric(int length);

// 将QByteArray转换为QJsonObject的函数
QJsonValue byteArrayToJson(const QByteArray& jsonData);

QString JsonObjectToString(const QJsonObject& jsonObject);

#ifndef MB
#define MB 1024*1024
#endif

#ifndef KB
#define KB 1024
#endif

#ifndef FULL_SCREEN_DEFAULT_PATH
#define FULL_SCREEN_DEFAULT_PATH "/root/.config/OICQ/client/temp/fullscreen_shot_temp.jpg"
#endif

#ifndef PRINT_SCREEN_DEFAULT_PATH
#define PRINT_SCREEN_DEFAULT_PATH "/root/.config/OICQ/client/temp/screen_shot_temp.jpg"
#endif

#ifndef STR_IMAGE_TYPE
#define STR_IMAGE_TYPE "![image]("
#endif

#ifndef FILE3_TYPE
#define FILE3_TYPE "file:///"
#endif

#ifndef FILE4_TYPE
#define FILE4_TYPE "file:////"
#endif

//简化属性定义(可读可写属性)
#define Q_PROPERTY_AUTO(TYPE, NAME) \
Q_PROPERTY(TYPE NAME MEMBER m_##NAME NOTIFY NAME##Changed) \
    public: \
    Q_SIGNALS: \
    void NAME##Changed(); \
    private: \
    TYPE m_##NAME;

//简化属性定义(只读属性)
#define Q_PROPERTY_RDONLY_AUTO(TYPE, NAME) \
Q_PROPERTY(TYPE NAME READ NAME NOTIFY NAME##Changed) \
    private: \
    TYPE m_##NAME; \
    public: \
    Q_SIGNALS: \
    void NAME##Changed(); \
    public: \
    TYPE NAME() const { \
        return m_##NAME; \
    }

#endif // GLOBAL_H
