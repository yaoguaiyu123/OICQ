#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H
#include <QAbstractListModel>
#include <QDateTime>
#include "global.h"

// model类，提供接口给qml
// 此类为单例类

class FileClient;
class FriendData;
class FriendModelPrivate;
class TcpSocket;
class QImage;
#include <QQmlEngine>
#include "messagemodel.h"
#include <QQuickTextDocument>
#include <QtQml/qqmlregistration.h>

class FriendModel : public QAbstractListModel {
    Q_OBJECT
    QML_SINGLETON
    QML_NAMED_ELEMENT(FriendModel)
    Q_PROPERTY_RDONLY_AUTO(QString, currentHeadpath)
    Q_PROPERTY_RDONLY_AUTO(QString, currentName)
    Q_PROPERTY_RDONLY_AUTO(QString, myImagePath)
    Q_PROPERTY_RDONLY_AUTO(int, currentIndex)   //当前的好友索引
public:
    FriendModel(QAbstractListModel*);
    static FriendModel& singleTon();

    static FriendModel* create(QQmlEngine* qmlengine, QJSEngine*) {
        FriendModel* object = &singleTon();
        object->m_jsvalue = qmlengine->newSymbol(QString("FriendModel"));
        return object;
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    MessageModel* updateMessageModel(int index);
    void sendMessage(QString,int,int);
    void downloadFileRequest(int friendiIndex, int messageIndex,const QString& filepath);
    void updateMyHead(QString);
    void cancelUploadOrDownload(int friendIndex,int messageIndex);
    QList<QString> currentWindowImages(int friendIndex, int messageIndex);
    void addNewFriend(const QJsonValue& jsonvalue,const QImage& image);
    void addNewFriend(QString username, qint64 userid, QString headpath);
    void addFriends(const QJsonValue &,const QList<QImage> &);
    void updateHaveSizeAndRecvSize(int friendIndex,int messageIndex,qint64 haveRW,qint64 toRW);
signals:
    void initDataFinished();
    void newMessage(int);
    void toManyFiles();
    void sigdownloadFile(qint64 messageId, qint64 from, qint64 to, const QString& filepath);
    void siguploadFile(const QString& filePath, qint64 from ,qint64 to,qint64 messageId);
private slots:
    void onLoginReturn(const QImage &image);
private:
    Q_DISABLE_COPY(FriendModel)
    QJSValue m_jsvalue;

    FriendData* _allData = nullptr;
    MessageModel* _messageModel = nullptr;
    QHash<int, QByteArray> m_role; //表头
    TcpSocket* m_tcpsocket = nullptr;
    QList<FileClient*> m_fileList;
};

#endif // FRIENDMODEL_H
