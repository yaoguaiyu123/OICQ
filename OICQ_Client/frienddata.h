#ifndef FRIENDDATA_H
#define FRIENDDATA_H
#include <QList>
#include <QByteArray>
#include <QHash>
#include <QJsonValue>
#include <QImage>
#include "global.h"

// 数据类，为单纯的c++类
// 此类为单例类
class FriendData : public QObject{
    Q_OBJECT
    //将私有成员对友元类开放
    friend class FriendModel;
    friend class MessageModel;
private:

public:
    FriendData(QObject * parent = nullptr);
    ~FriendData();
    static FriendData& singleTon();
    void addNewFriend(const QJsonValue& jsonvalue,const QImage& image);
    void addNewFriend(QString username, qint64 userid, QString headpath);
private:
    void clear();

    // 保证m_friends通过index来访问m_messages获得匹配的数据
    // 使用List管理好友和消息
    QList<Friend> friends;  //好友数据
    QList<QList<Recode>> messages; //多个好友消息数据
};



#endif // FRIENDDATA_H
