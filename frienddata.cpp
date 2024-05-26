#include "frienddata.h"
#include <QDateTime>
#include <QString>
#include "global.h"
#include <QDebug>
/// 数据类

namespace {

QString headCachePath = "/root/.config/OICQ/client/head";

}

FriendData::FriendData(QObject * parent)
    :QObject(parent)
{
    ;
}

FriendData::~FriendData()
{
}

//TODO
void FriendData::clear()
{
    int count = friends.size();
    if (count > 0) {
        friends.clear();
    }
}

void FriendData::addNewFriend(QJsonValue& jsonvalue, QImage& image)
{
    QJsonObject obj = jsonvalue.toObject();
    QString username = obj.value("username").toString();
    qint64 userId = obj.value("userId").toInteger();

    Friend f;
    f.name = username;
    f.userid = userId;
    QString headpath = headCachePath + "/" + generateRandomAlphanumeric(10) + ".jpg";
    if (!image.save(headpath)) {
        qDebug() << "保存失败  " << headpath;
    }
    f.headPath = "file:///" + headpath;
    friends.append(f);

}

void FriendData::addNewFriend(QString username, qint64 userid, QString headpath)
{
    // qDebug() << "添加好友到model";
    Friend f;
    f.name = username;
    f.userid = userid;
    if (headpath.mid(0, 4) == "file") {
        f.headPath = headpath;
    }else{
        f.headPath = "file:///" + headpath;
    }
    friends.append(f);
}

//返回单例对象的方法
FriendData& FriendData::singleTon()
{
    static FriendData friendData;
    return friendData;
}
