#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>

// model类，提供接口给qt 为单例类，但是单例的保证由friendModel执行
class FriendData;
struct Recode;

class MessageModel : public QAbstractListModel {
    Q_OBJECT
    QML_SINGLETON
    QML_NAMED_ELEMENT(MessageModel)
public:
    explicit MessageModel(QObject* parent);
    static MessageModel& singleTon();
    static MessageModel* create(QQmlEngine* qmlengine, QJSEngine*) {
        MessageModel* object = &singleTon();
        object->m_jsvalue = qmlengine->newSymbol(QString("MessageModel"));
        return object;
    }

    ~MessageModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    void setModelData(QList<Recode>* data);
    void addMessageList(QList<Recode>& messgeList, int index = -1);
    void addMessage(qint64 id,QString text,QString msgType,int index = -1,qint64 userid = -1);
    void addMessage(qint64 id,QString text, QString msgType, QString filename, QString filesize, int index = -1, qint64 userid = -1);
public slots:
    void updateHaveSizeAndRecvSize(int index, qint64 haveSize, qint64 totalSize);
private:
    QJSValue m_jsvalue;

    QList<Recode>* _currentData = nullptr;
    FriendData* _allData = nullptr;
};

#endif // MESSAGEMODEL_H
