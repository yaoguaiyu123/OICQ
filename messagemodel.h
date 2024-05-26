#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>

// model类，提供接口给qt 为单例类，但是单例的保证由friendModel执行
class FriendData;
struct Recode;

class MessageModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit MessageModel(QObject* parent = nullptr);
    ~MessageModel();
    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    void setData(QList<Recode>* data);
    void addMessageList(QList<Recode>& messgeList, int index = -1);
    void addMessage(QString text,QString msgType,int index = -1,qint64 userid = -1);
private:
    QList<Recode>* _currentData;
    FriendData* _allData;
};

#endif // MESSAGEMODEL_H
