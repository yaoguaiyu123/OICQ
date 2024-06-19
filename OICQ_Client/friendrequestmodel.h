#ifndef FRIENDREQUESTMODEL_H
#define FRIENDREQUESTMODEL_H

#include <QAbstractListModel>
#include "global.h"
// 好友添加请求列表的model
class TcpSocket;
class FriendModel;
class FriendRequestModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit FriendRequestModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    static FriendRequestModel& singleTon();
    Q_INVOKABLE void choseAddFriend(int, int);
private:
    QList<Friend> friends;
    TcpSocket* _tcpSocket;
    FriendModel* _friendModel;
};

#endif // FRIENDREQUESTMODEL_H
