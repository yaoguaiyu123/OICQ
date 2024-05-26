#include "messagemodel.h"
#include "frienddata.h"
#include "global.h"

///消息列表model接口

MessageModel::MessageModel(QObject* parent)
    :_allData(&FriendData::singleTon()),
    QAbstractListModel { parent }
{
    ;
}


MessageModel::~MessageModel()
{
    ;
}

int MessageModel::rowCount(const QModelIndex& parent) const
{
    // qDebug() << "messageModel的count" << _currentData->size();
    return _currentData->size();
}

QVariant MessageModel::data(const QModelIndex& index, int role) const{
    if (!index.isValid() || index.row() >= _currentData->size()) {
        return QVariant("");
    }
    Recode f = _currentData->at(index.row());
    if (role == Qt::UserRole){
        return f.message;
    }else if (role == Qt::UserRole + 1){
        return f.type;
    }else if (role == Qt::UserRole + 2){
        return f.date;
    }
    return QVariant("");
}

QHash<int, QByteArray> MessageModel::roleNames() const{
    QHash<int, QByteArray> roles;
    roles[Qt::UserRole] = "msg";
    roles[Qt::UserRole + 1] = "type";
    roles[Qt::UserRole + 2] = "date";
    return roles;
}

void MessageModel::setData(QList<Recode>* data)
{
    beginResetModel();   //刷新数据
    _currentData = data;

    endResetModel();
}

// 添加MessageList
void MessageModel::addMessageList(QList<Recode>& messgeList, int index)
{
    beginResetModel();
    if(index == -1){
        //直接在最后添加一个新的
        _allData->messages.append(messgeList);
        if(_allData->messages.length() == 1){
            setData(&_allData->messages.first());
        }
    }
    endResetModel();
}

//添加对应friendModel的index的message
void MessageModel::addMessage(QString text, QString msgType, int index, qint64 userid)
{
    beginResetModel();
    QString now = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm");
    if(index != -1){
        _allData->messages[index].append({ now, msgType, text });
    } else {
        for (int i = 0; i < _allData->friends.length(); ++i) {
            if (_allData->friends[i].userid == userid) {
                _allData->messages[i].append({ now, msgType, text });
                break;
            }
        }
    }
    endResetModel();
}

