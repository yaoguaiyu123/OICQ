#ifndef FRIENDREQUESTMODEL_H
#define FRIENDREQUESTMODEL_H

#include <QAbstractListModel>
#include "global.h"
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>

// 好友添加请求列表的model
class TcpSocket;
class FriendModel;
class FriendRequestModel : public QAbstractListModel {
    Q_OBJECT
    QML_SINGLETON
    QML_NAMED_ELEMENT(FriendRequestModel)
public:
    explicit FriendRequestModel(QObject* parent);
    static FriendRequestModel& singleTon();

    static FriendRequestModel* create(QQmlEngine* qmlengine, QJSEngine*) {
        FriendRequestModel* object = &singleTon();
        object->m_jsvalue = qmlengine->newSymbol(QString("FriendRequestModel"));
        return object;
    }

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    void choseAddFriend(int, int);
private:
    QJSValue m_jsvalue;
    QList<Friend> friends;
    TcpSocket* _tcpSocket = nullptr;
    FriendModel* _friendModel = nullptr;
};

#endif // FRIENDREQUESTMODEL_H
