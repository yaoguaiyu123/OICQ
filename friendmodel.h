#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H
#include <QAbstractListModel>
#include <QDateTime>
#include "global.h"
// model类，提供接口给qt
// 此类为单例类

class FriendData;
class FriendModelPrivate;
class MessageModel;
class TcpSocket;
class QImage;
class FriendModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY_RDONLY_AUTO(QString, currentHeadpath)
    Q_PROPERTY_RDONLY_AUTO(QString, currentName)
    Q_PROPERTY_RDONLY_AUTO(QString, myImagePath)
    Q_PROPERTY_RDONLY_AUTO(int, currentIndex)   //当前的好友索引
public:
    FriendModel(QAbstractListModel* parent = nullptr);
    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE MessageModel* getMessageModel(int index = -1);
    Q_INVOKABLE void sendMessage(QString,int,int);
    static FriendModel& singleTon();
    Q_INVOKABLE void updateMyHead(QString);
    void addNewFriend(QJsonValue& jsonvalue, QImage& image);
    void addNewFriend(QString username, qint64 userid, QString headpath);
    void addFriends(QJsonValue &, QList<QImage> &);
signals:
    void initDataFinished();
    void newMessage(int);
private:
    FriendData* _allData;
    MessageModel* _messageModel;
    QHash<int, QByteArray> m_role; //表头
    TcpSocket* m_tcpsocket;
};

#endif // FRIENDMODEL_H
