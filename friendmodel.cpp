#include "friendmodel.h"
#include <QDebug>
#include <QDateTime>
#include "frienddata.h"
#include "messagemodel.h"
#include "network/tcpsocket.h"
#include <QList>
#include "global.h"
#include <QImage>
#include <QDir>
#include <QTimer>
#include "network/fileclient.h"
#include <QRandomGenerator>
#include <QThread>
#include <QJsonArray>
#include <QRegularExpression>

///好友列表model接口

namespace {
void processImages(QString content, QList<QImage>& imageList)
{
    qDebug() << "要处理的消息 " << content;
    for (qint32 i = 0; i < content.length(); ++i) {
        if (content.mid(i, 9) == "![image](") {
            // 是一个图片
            i = i + 9;
            if (content.mid(i, 8) == "file:///") {
                i = i + 8;
                QString url = "/";
                while (content[i] != ')' && i < content.length()) {
                    url += content[i];
                    ++i;
                }
                QImage image(url);
                imageList.append(image);
            }
        }
    }
}

// 生成消息随机ID的函数

qint64 generateMessageId()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    int year = currentTime.date().year();
    int month = currentTime.date().month();
    int day = currentTime.date().day();
    int hour = currentTime.time().hour();
    int minute = currentTime.time().minute();
    int second = currentTime.time().second();
    qint64 randomPart = QRandomGenerator::global()->generate() % 100000000;
    qint64 uniqueId = (year % 100) * 10000000000 + month * 100000000 + day * 1000000 + hour * 10000 + minute * 100 + second;
    uniqueId = (uniqueId << 8) | randomPart;
    return uniqueId;
}




QString processMessageWithImages(QString message, const QString &defaultImagePath) {
    QRegularExpression regex(R"(!\[image\]\((file:///.*?)\))"); // 正则表达式匹配 file:// 开头的图片 URL
    QRegularExpressionMatchIterator i = regex.globalMatch(message);

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString strurl = match.captured(1);
        QUrl url(strurl);
        QImage image(url.toLocalFile());

        if (image.isNull()) {
            // 图片无法加载，替换为默认图片路径
            message.replace(strurl, defaultImagePath);
        }
    }

    return message;
}


QString imageCachePath = "/root/.config/OICQ/client/recv";
QString headCachePath = "/root/.config/OICQ/client/head";
}

FriendModel::FriendModel(QAbstractListModel* parent)
    :_allData(&FriendData::singleTon()),
    _messageModel(new MessageModel(this)),
    m_tcpsocket(&TcpSocket::singleTon()),
    QAbstractListModel(parent)
{
    //初始化表头
    m_role.insert(Qt::UserRole, "name");
    m_role.insert(Qt::UserRole + 1, "headPath");
    m_role.insert(Qt::UserRole + 2, "recentMessage");
    m_currentIndex = 0;
    m_currentName = "";
    m_currentHeadpath = "";

    //好友列表
    QObject::connect(m_tcpsocket, &TcpSocket::friendListReturn,
        [this](QJsonValue& value, QList<QImage> imagelist) {
            addFriends(value ,imagelist);
            if(!_allData->messages.isEmpty()){
                _messageModel->setModelData(&_allData->messages[m_currentIndex]); // 设置当前聊天窗口为第一个
                m_currentName = _allData->friends.at(m_currentIndex).name;
                m_currentHeadpath = _allData->friends.at(m_currentIndex).headPath;
                emit currentNameChanged();
                emit currentHeadpathChanged();   //这里发出的信号不需要主动连接，而是qml自动连接
                emit initDataFinished();
            }
        });

    // 接收到私发消息
    QObject::connect(m_tcpsocket, &TcpSocket::privateMessageReturn,
        [this](QJsonValue& value,QList<QImage>images) {
            QJsonObject obj;
            if (value.isObject()) {
                obj = value.toObject();
            }
            qint64 from = obj.value("from").toInteger();
            QString message = obj.value("message").toString();
            if (!images.isEmpty()) {
                message.replace("client/send", "client/recv"); // 转变地址
                // 给所有收到的图片建立缓存
                int noCount = 0;
                for (qint32 i = 0; i < message.length(); ++i) {
                    if (message.mid(i, 9) == "![image](") {
                        // 是一个图片
                        i = i + 9;
                        if (message.mid(i, 8) == "file:///") {
                            i = i + 8;
                            QString url = "/";
                            QString dateFolder; // 日期对应的文件夹
                            while (message[i] != ')' && i < message.length()) {
                                url += message[i];
                                if (message.mid(i, 4) == "recv") {
                                    url += "ecv/";
                                    i += 5;
                                    dateFolder = message.mid(i, 8);
                                    url += dateFolder + "/";
                                    i += 8;
                                }
                                ++i;
                            }
                            //保存图片到缓存
                            QDir dir(imageCachePath);
                            if (!dir.exists(dateFolder)) {
                                dir.mkpath(dateFolder);
                            }
                            qDebug() << "要保存的地址 :" << url;
                            if (!images.at(noCount).save(url)) {
                                return;
                            }
                        }
                        ++noCount;   //图片+1
                    }
                }
            }
            //刷新qml的回显
            int j;
            for (int i = 0; i < _allData->friends.length(); ++i) {
                if (_allData->friends.at(i).userid == from) {
                    QModelIndex modelindex = createIndex(i, 0);
                    emit dataChanged(modelindex, modelindex, {Qt::UserRole + 2});
                    _messageModel->addMessage(-1,message, "recv", i);
                    emit dataChanged(modelindex, modelindex, {Qt::UserRole + 2});
                    j = i;
                    break;
                }
            }
            emit(newMessage(j));
        });

    //登录返回
    QObject::connect(m_tcpsocket, &TcpSocket::loginReturn,
        [this](int res,QImage& image) {
            if (res == Success) {
                !image.save(headCachePath + "/myHead.jpg");
                m_myImagePath =  "file://" + headCachePath + "/myHead.jpg";
            }
        });

    //更新头像返回
    QObject::connect(m_tcpsocket, &TcpSocket::updateHeadReturn,
        [this](const QImage& image) {
            !image.save(headCachePath + "/myHead.jpg");
            m_myImagePath =  "qrc:/image/default_head.png";
            emit(myImagePathChanged());
            QTimer::singleShot(100, [this](){
                m_myImagePath =  "file://" + headCachePath + "/myHead.jpg";
                emit(myImagePathChanged());
            });
        });

    // 添加好友结果
    QObject::connect(m_tcpsocket, &TcpSocket::addFriendRes,
        [this](QJsonValue jsonvalue, QList<QImage>& images) {
            beginInsertRows(QModelIndex(), rowCount(), rowCount());
            addNewFriend(jsonvalue, images.first());
            endInsertRows();
        });

    // 接收到文件消息
    QObject::connect(m_tcpsocket, &TcpSocket::fileMessage,
        [this](QJsonValue jsonvalue) {
            if (!jsonvalue.isObject()) {
                return;
            }
            QJsonObject recvObj = jsonvalue.toObject();
            qint64 from = recvObj.value("from").toInteger();
            qint64 messageId = recvObj.value("messageId").toInteger();
            QString filename = recvObj.value("filename").toString();
            QString filesize = recvObj.value("filesize").toString();
            int j;
            for (int i = 0; i < _allData->friends.length(); ++i) {
                if (_allData->friends.at(i).userid == from) {
                    QModelIndex modelindex = createIndex(i, 0);
                    emit dataChanged(modelindex, modelindex, {Qt::UserRole + 2});   //刷新最近消息
                    _messageModel->addMessage(messageId, "[文件] " + filename , "recvfile", filename, filesize, i);
                    emit dataChanged(modelindex, modelindex, {Qt::UserRole + 2});
                    j = i;
                    break;
                }
            }
            emit(newMessage(j));
        });

    // 历史聊天消息 TODO 增加初始化速度
    QObject::connect(m_tcpsocket, &TcpSocket::historyMessageList,
        [this](QJsonValue& jsonvalue) {
            QJsonArray jsonArray = jsonvalue.toArray();
            qint64 userid = m_tcpsocket->getUserId();
            QModelIndex modelindex1 = createIndex(0, 0);
            QModelIndex modelindex2 = createIndex(rowCount() - 1, 0);

            emit dataChanged(modelindex1, modelindex2, { Qt::UserRole + 2});

            for (const QJsonValue& jsonvalue : jsonArray) {
                QJsonObject jsonobj = jsonvalue.toObject();
                qint64 senderId = jsonobj.value("senderId").toInteger();
                qint64 receiverId = jsonobj.value("receiverId").toInteger();
                qint64 messageId = jsonobj.value("messageId").toInteger(-1);
                QString message = jsonobj.value("message").toString();

                QString filename = jsonobj.value("filename").toString("");
                QString filesize = jsonobj.value("filesize").toString("");
                QString type = jsonobj.value("messageType").toString();
                if (type == "file") {
                    // 文件消息
                    if (senderId == userid) {
                        qDebug() << "你好世界1";
                        _messageModel->addMessage(messageId, message, "sendfile", filename, filesize, -1, receiverId);
                    }else{
                        message.replace("client/send", "client/recv"); // 对message转变地址
                        message = processMessageWithImages(message, "qrc:/icon/fail_to_load.png");
                        _messageModel->addMessage(messageId, message, "recvfile", filename, filesize, -1, senderId);
                    }
                }else{
                    //普通消息
                    if (senderId == userid) {
                        _messageModel->addMessage(messageId, message, "send", -1, receiverId);
                    }else{
                        qDebug() << "你好世界2";
                        message.replace("client/send", "client/recv"); // 对message转变地址
                        message = processMessageWithImages(message, "qrc:/icon/fail_to_load.png");
                        _messageModel->addMessage(messageId, message, "recv", -1, senderId);
                    }
                }

            }
            emit dataChanged(modelindex1, modelindex2, { Qt::UserRole + 2});
            _messageModel->setModelData(&_allData->messages[m_currentIndex]);
            emit(newMessage(m_currentIndex));   //通知list移动到最底端

        });
}

//在delegate能获得数据
QVariant FriendModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= _allData->friends.size()) {
        return QVariant("");
    }
    Friend f = _allData->friends.at(index.row());
    QList<Recode> recodeList = _allData->messages.at(index.row());
    if (role == Qt::UserRole) {
        // qDebug() << f->name;
        return f.name;
    } else if (role == Qt::UserRole + 1){
        return f.headPath;
    } else if (role == Qt::UserRole + 2) {
        return recodeList.last().message;
    }
    return QVariant("");
}


int FriendModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return _allData->friends.size();
}

QHash<int, QByteArray> FriendModel::roleNames() const
{
    return m_role;
}

FriendModel& FriendModel::singleTon()
{
    static FriendModel friendModel;
    return friendModel;
}

//返回另一个模型的时候需要以指针的形式返回
MessageModel* FriendModel::getMessageModel(int index)
{
    if (index != -1) {
        m_currentIndex = index;
        m_currentName = _allData->friends.at(m_currentIndex).name;
        m_currentHeadpath = _allData->friends.at(m_currentIndex).headPath;
        emit(currentHeadpathChanged());
        emit(currentNameChanged());
        if(m_currentIndex < _allData->messages.length()){
            _messageModel->setModelData(&_allData->messages[m_currentIndex]);
        }
    }
    return _messageModel;
}

// 初始化好友列表
void FriendModel::addFriends(QJsonValue& jsonvalue, QList<QImage>& imagelist)
{
    QJsonArray array = jsonvalue.toArray();
    if (array.isEmpty()) {
        return;
    }
    beginResetModel();
    for (int i = 0; i < array.size(); ++i) {
        QJsonValue value = array.at(i);
        if (!value.isObject()) {
            return;
        }
        QJsonObject obj = value.toObject();
        qint64 userid = obj.value("userId").toInteger();
        QString name = obj.value("username").toString();
        QString headpath = headCachePath + "/" + generateRandomAlphanumeric(10) + ".jpg";
        imagelist.at(i).save(headpath);
        Friend f;
        f.userid = userid;
        f.name = name;
        f.headPath = "file://" + headpath;
        QList<Recode> single_messages;
        QString now = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm");
        single_messages.append({-1, now, "tipDate", "" });
        single_messages.append({-1, now, "tip", "快来与你的好友" + f.name + "聊天吧" }); // 这边使用列表初始化struct
        _allData->friends.append(f);
        _messageModel->addMessageList(single_messages);   //调用_messageModel的方法进行添加后刷新
    }
    endResetModel();
}

//发送消息
void FriendModel::sendMessage(QString message,int index,int type){
    // qDebug() << "###############    " << message << " " << type;
    qint64 msgId = generateMessageId();
    if (type == PrivateMessage) {
        QList<QImage> imageList;
        processImages(message, imageList);  //获取所有图片
        QModelIndex modelIndex = createIndex(m_currentIndex, 0, {Qt::UserRole + 2});
        emit dataChanged(modelIndex, modelIndex, {Qt::UserRole + 2});    //刷新最近消息
        _messageModel->addMessage(msgId, message, "send", index);
        emit dataChanged(modelIndex, modelIndex, {Qt::UserRole + 2});
        // 发送消息到socket（先得到friendId）
        qint64 userId = _allData->friends.at(index).userid;
        QJsonObject obj;
        obj.insert("to", userId);
        // obj.insert("messageId", msgId);  //消息ID
        obj.insert("message", message);
        if (imageList.isEmpty()) {
            m_tcpsocket->packingMessage(JsonObjectToString(obj), PrivateMessage);
        } else {
            m_tcpsocket->packingMessage(JsonObjectToString(obj), PrivateMessage, imageList);
        }
    } else if (type == FileMessage) {
        QStringList files = message.split(","); // 分割字符串
        if (files.length() > 5 - m_fileList.length()) {
            emit(toManyFiles());
            return;
        }
        qint64 from = m_tcpsocket->getUserId();
        qint64 to = _allData->friends.at(index).userid;

        beginResetModel();
        for (const QString& filepath : files) {
            QFileInfo fileInfo(filepath.mid(7));
            QString fileName = fileInfo.fileName();
            // qDebug() << "得到文件大小" << fileName << "   " << fileInfo.size() << "   " << filepath.mid(7);
            qint64 totalSize = fileInfo.size();
            double kilobytes = totalSize / 1024.0;
            double megabytes = totalSize / 1048576.0; // 1024*1024

            QString result;
            if (totalSize >= 1048576) {
                result = QString::number(megabytes, 'f', 2) + " MB";
            } else if (totalSize >= 1024) {
                result = QString::number(kilobytes, 'f', 2) + " KB";
            } else {
                result = QString::number(totalSize) + " B";
            }


            // 上传文件
            _messageModel
                ->addMessage(msgId, "[文件] " + fileName, "sendfile", fileName, result, index);

            FileClient* client = new FileClient(index, _messageModel->rowCount() - 1);

            m_fileList.append(client);  //加入list进行管理


            QThread* thread = new QThread();
            client->moveToThread(thread);
            thread->start();
            connect(this, &FriendModel::siguploadFile, client, &FileClient::uploadFile);
            connect(client, &FileClient::destroyed, thread, &QThread::quit);
            connect(client, &FileClient::destroyed,[this, thread](){
                qDebug() << "接收到发送结束的信号，这里在进行list中的remove以及thread的退出";
                m_fileList.removeOne(qobject_cast<FileClient*>(sender()));
                thread->quit();  //停止线程
            });
            connect(client, &FileClient::updateFileMessage, this, &FriendModel::updateHaveSizeAndRecvSize);
            connect(thread, &QThread::finished, thread, &QThread::deleteLater);  //释放线程资源

            emit siguploadFile(filepath, from, to, msgId);  //通过信号调用
        }
        endResetModel();
    }
    emit(newMessage(m_currentIndex));
}

// 文件下载请求
void FriendModel::downloadFileRequest(int friendiIndex, int messageIndex, const QString& filepath)
{
    qDebug() << "你好世界";
    qint64 from = _allData->friends.at(friendiIndex).userid;
    qint64 to = m_tcpsocket->getUserId();
    qint64 messageId = _allData->messages.at(friendiIndex).at(messageIndex).id;
    QString msgtype = _allData->messages.at(friendiIndex).at(messageIndex).type;
    _allData->messages[friendiIndex][messageIndex].haveRecvOrSendSize = 0;   // 重置为0

    if (msgtype == "sendfile") { // 下载自己的文件
        qint64 t = from;
        from = to;
        to = t;
    }
    qDebug() <<" 下载文件的请求          " << from << " " << to << " " << messageId << " " << msgtype;
    FileClient* client = new FileClient(friendiIndex , messageIndex);
    m_fileList.append(client);  //加入list进行管理

    QThread* thread = new QThread();
    client->moveToThread(thread);
    thread->start();
    connect(client, &FileClient::destroyed,[this, thread](){
        qDebug() << "接收到发送结束的信号，这里在进行list中的remove以及thread的退出";
        m_fileList.removeOne(qobject_cast<FileClient*>(sender()));
        thread->quit();  //停止线程
    });
    connect(client, &FileClient::updateFileMessage, this, &FriendModel::updateHaveSizeAndRecvSize);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);  //释放线程资源
    connect(this, &FriendModel::sigdownloadFile, client, &FileClient::downloadFile);
    emit(sigdownloadFile(messageId, from, to, filepath));
}

// 更新头像
void FriendModel::updateMyHead(QString surl)
{
    // file3类型的地址
    QUrl url(surl);
    QString path = url.toLocalFile();
    qDebug() << "更新头像: path = " << path;
    QImage image(path);
    if (image.isNull()) {
        qDebug() << "头像无效，更新失败";
        return;
    }
    QList<QImage> list;
    list.append(image);
    m_tcpsocket->packingMessage("{}", UpdateHead, list);
}

//添加一个新好友
void FriendModel::addNewFriend(QJsonValue& jsonvalue, QImage& image){
    if (!jsonvalue.isObject()) {
        return;
    }
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _allData->addNewFriend(jsonvalue,image);
    QList<Recode> single_messages;
    QString now = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm");
    single_messages.append( {-1,now, "tipDate", "" });
    single_messages.append( {-1, now, "tip", jsonvalue.toObject().value("username").toString() +
            "已经是你的好友了，开始聊天吧" }); // 这边使用列表初始化struct
    _messageModel->addMessageList(single_messages);
    if (_allData->messages.length() == 1) {
        emit(initDataFinished());   //如果是第一个好友就要初始化
    }
    endInsertRows();
}

//添加一个新好友
void FriendModel::addNewFriend(QString username, qint64 userid, QString headpath){
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _allData->addNewFriend(username,userid,headpath);
    QList<Recode> single_messages;
    QString now = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm");
    single_messages.append({-1, now, "tipDate","" });
    single_messages.append({-1, now, "tip", username +
            "已经是你的好友了，开始聊天吧" }); // 这边使用列表初始化struct
    _messageModel->addMessageList(single_messages);
    if (_allData->messages.length() == 1) {
        emit(initDataFinished());   //如果是第一个好友就要初始化
    }
    endInsertRows();
}

// 更新文件显示的值
void FriendModel::updateHaveSizeAndRecvSize(int friendIndex,
                                            int messageIndex,
                                            qint64 haveRW,
                                            qint64 toRW)
{
    if (friendIndex != m_currentIndex) {
        Recode& recode = _allData->messages[friendIndex][messageIndex];
        recode.haveRecvOrSendSize = haveRW;
        recode.fileTotalSize = toRW;
    } else {
        _messageModel->updateHaveSizeAndRecvSize(messageIndex, haveRW, toRW);
    }
}

// 取消上传或下载
void FriendModel::cancelUploadOrDownload(int friendIndex,int messageIndex){
    for(FileClient * client : m_fileList){
        if(client->getIndex() == friendIndex && client->getMessageIndex() == messageIndex){
            client->cancelTransfer();
            updateHaveSizeAndRecvSize(friendIndex, messageIndex, 0, 9999);
        }
    }
}


