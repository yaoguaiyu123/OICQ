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
