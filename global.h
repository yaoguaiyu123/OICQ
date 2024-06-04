#ifndef GLOBAL_H
#define GLOBAL_H
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QList>
#include <QByteArray>

enum MSG_TYPE {
    PrivateMessage,
    FileMessage,
    AddFriend,
    Login,
    FriendList,
    UpdateHead,
    AddFriendRequest,
    AddFriendRes,
    FriendRequestList
};

enum RETURN{
    Repeat,
    Success,
    Fail,
    Repeat2
};

struct Recode {
    QString date;
    QString type;
    QString message;
};

struct Friend {
    QString name;
    QString headPath;
    qint64 userid; // QQ号
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
