#include "clienthandler.h"
#include <QByteArray>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <QtEndian>
#include "global.h"
#include "database/dbmanager.h"
#include <QBuffer>
#include <QImage>
#include "chatmemberdata.h"

ClientHandler::ClientHandler(qintptr socketDescriptor, QObject* parent)
    : QObject(parent)
    , m_descriptor(socketDescriptor)
{
    m_sendbuf = new uchar[32 * 1024 * 1024];
}

// 初始化
void ClientHandler::init()
{
    qDebug() << "初始化套接字.......";
    m_tcpSocket = new QTcpSocket();
    m_tcpSocket->setSocketDescriptor(m_descriptor);
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &ClientHandler::on_ready_read);
    connect(m_tcpSocket, &QTcpSocket::disconnected,[this]() {
        emit(disconnected(this,m_isLogin));
    });
}

qint64 ClientHandler::userId()
{
    return m_userId;
}

// 将信息打包成为一个MESG对象
void ClientHandler::packingMessage(QJsonValue value, int msgType,  QList<QImage> imageList)
{
    // qDebug() << "打包对象: " << value;
    QJsonDocument doc;
    if (value.isObject()) {
        doc = QJsonDocument(value.toObject());
    } else if (value.isArray()){
        doc = QJsonDocument(value.toArray());
    } else {
        return;
    }
    QByteArray bytes = doc.toJson();
    MESG* message = new MESG();
    message->msg_type = static_cast<MSG_TYPE>(msgType);
    message->len = bytes.size();
    message->data = new uchar[message->len];
    std::memcpy(message->data, bytes.data(), message->len);
    //打包图片
    if (!imageList.isEmpty()) {
        for (const QImage& image : imageList) {
            QByteArray buffer;
            QBuffer qBuffer(&buffer);
            qBuffer.open(QIODevice::WriteOnly);

           // 将图片以PNG格式转换并存储到buffer中
            if (!image.save(&qBuffer, "PNG")) {
                qDebug() << "图片转换失败";
                continue;
            }
            message->imagesData.append(buffer);
        }
    }

    sendToClient(message);
}

//发送数据的函数
void ClientHandler::sendToClient(MESG* send)
{
    // qDebug() << "开始回送数据到客户端";
    if (m_tcpSocket->state() == QAbstractSocket::UnconnectedState) {
        if (send->data)
            free(send->data);
        if (send)
            free(send);
        return;
    }

    quint32 bytestowrite = 0;
    m_sendbuf[bytestowrite++] = '$'; // 开头
    bytestowrite += 4; //为包大小预留空间
    qToBigEndian<quint16>(send->msg_type, m_sendbuf + bytestowrite); // quint16 决定了存入两个字节的数据
    bytestowrite += 2;
    qToBigEndian<quint64>(m_userId, m_sendbuf + bytestowrite);
    bytestowrite += 8;
    qToBigEndian<quint32>(send->len, m_sendbuf + bytestowrite);
    bytestowrite += 4;
    memcpy(m_sendbuf + bytestowrite, send->data, send->len);
    bytestowrite += send->len;

           // 图片数量
    qToBigEndian<quint32>(send->imagesData.size(), m_sendbuf + bytestowrite);
    bytestowrite += 4;

           // 图片数据
    for (const QByteArray &imgData : send->imagesData) {
        qToBigEndian<quint32>(imgData.size(), m_sendbuf + bytestowrite);
        bytestowrite += 4;
        memcpy(m_sendbuf + bytestowrite, imgData.constData(), imgData.size());
        bytestowrite += imgData.size();
    }

    m_sendbuf[bytestowrite++] = '#'; // 结尾
    qToBigEndian<quint32>(bytestowrite, m_sendbuf + 1);   //包大小

    qint64 hastowrite = bytestowrite;
    qint64 ret = 0, haswrite = 0;
    do {
        ret = m_tcpSocket->write((char*)m_sendbuf + haswrite, hastowrite - haswrite);
        m_tcpSocket->waitForBytesWritten();
        if (ret == -1 &&  m_tcpSocket->error() == QAbstractSocket::TemporaryError) {
            ret = 0;
        } else if (ret == -1) {
            break;
        }
        haswrite += ret;
    } while (haswrite < hastowrite); // 确保所有数据都被写入

    qDebug() << bytestowrite << "   这是我发送的数据的总数!!!!!!!!!!";

    if (send->data) {
        free(send->data);
    }
    if (send) {
        free(send);
    }
}


// 接收数据的函数
void ClientHandler::on_ready_read()
{
    qDebug() << "开始读取数据";
    while (m_tcpSocket->bytesAvailable() > 0) {
        int aliasSize = m_tcpSocket->bytesAvailable();
        QByteArray datagram;
        datagram.resize(aliasSize);
        m_tcpSocket->read(datagram.data(), aliasSize); // 读取数据

        qDebug() << aliasSize << "    这是我接收的数据的总数";
        m_recvbuf.append(reinterpret_cast<const uchar*>(datagram.data()), aliasSize);
        parsePackage(); // 解析包
    }
}

// 接收数据的函数
/**
 1个字节的开头 $
 4个字节的包大小
 2个字节的 msg_type(大端格式)
 8个字节的 userid(大端格式)
 4个字节的 len(大端格式)
 len个字节的 data
 1个字节的结尾 #
*/
void ClientHandler::parsePackage()
{
    //解析数据包
    if (m_recvbuf.capacity - m_recvbuf.availableSpaceSize() < 15) {
        return; // 小于最小包长直接退出
    }
    qint32 pos = 0;
    uchar beginC = m_recvbuf.buffer[m_recvbuf.beginIndex + pos];
    ++pos;
    if (beginC != '$') {
        qDebug() << "开头错误 " << beginC;
        return;
    }
    quint32 packageSize = qFromBigEndian<quint32>(reinterpret_cast<const uchar*>(m_recvbuf.buffer + m_recvbuf.beginIndex + pos));
    if (m_recvbuf.dataSize() < packageSize) {
        return; // 小于包的长度直接退出
    }
    pos += sizeof(quint32); // 包大小
    quint16 msgType = qFromBigEndian<quint16>(reinterpret_cast<const uchar*>(m_recvbuf.buffer + m_recvbuf.beginIndex + pos));
    pos += sizeof(quint16); // 消息类型
    quint64 userId = qFromBigEndian<quint64>(reinterpret_cast<const uchar*>(m_recvbuf.buffer + m_recvbuf.beginIndex + pos));
    pos += sizeof(quint64); // id
    quint32 msgLen = qFromBigEndian<quint32>(reinterpret_cast<const uchar*>(m_recvbuf.buffer + m_recvbuf.beginIndex + pos));
    pos += sizeof(quint32); // json长度
    QByteArray msgData(reinterpret_cast<char*>(m_recvbuf.buffer + m_recvbuf.beginIndex + pos), msgLen);
    pos += msgLen; // json
    quint32 imgCount = qFromBigEndian<quint32>(reinterpret_cast<const uchar*>(m_recvbuf.buffer + m_recvbuf.beginIndex + pos));
    pos += sizeof(quint32); // 读取图片数量

           // 读取图片存储到images
    QList<QImage> images;
    for (quint32 i = 0; i < imgCount; ++i) {
        quint32 imgSize = qFromBigEndian<quint32>(reinterpret_cast<const uchar*>(m_recvbuf.buffer + m_recvbuf.beginIndex + pos));
        pos += sizeof(quint32);
        QByteArray imgData(reinterpret_cast<char*>(m_recvbuf.buffer + m_recvbuf.beginIndex + pos), imgSize);
        pos += imgSize;
        QImage image;
        if (image.loadFromData(imgData)) {
            images.append(image);
        }
    }

    if (m_recvbuf.buffer[m_recvbuf.beginIndex + pos] != '#') {
        qDebug() << "数据包结尾错误";
        return;
    }
    ++pos;
    m_recvbuf.beginIndex += pos;
    if (m_recvbuf.beginIndex >= m_recvbuf.capacity) {
        m_recvbuf.beginIndex -= m_recvbuf.capacity;
    }


    qDebug() << "msgType:" << msgType;
    switch (msgType) {
    case PrivateMessage:
        parsePrivateMessage(byteArrayToJson(msgData), images);
        break;
    case AddFriend:
        parseAddFriend(byteArrayToJson(msgData));
        break;
    case Login:
        parseLogin(byteArrayToJson(msgData));
        break;
    case UpdateHead:
        parseUpdateHead(userId, images);
        break;
    case AddFriendRes:
        parseAddFriendRes(byteArrayToJson(msgData));
        break;
    case UpdateIsRead:
        parseUpdateIsRead(byteArrayToJson(msgData));
        break;
    default:
        break;
    }

}

//私发消息
void ClientHandler::parsePrivateMessage(QJsonValue jsonvalue, QList<QImage> images)
{
    // qDebug() << "socket接收到私发消息";
    emit(forwardMessages(jsonvalue, m_userId,images));
}

//登录
void ClientHandler::parseLogin(QJsonValue jsonvalue)
{
    QJsonObject object = jsonvalue.toObject();
    qint64 userid = object.value("userId").toInteger();
    QString password = object.value("password").toString();
    QJsonObject sendObj;
    QVariantMap vmap = DBManager::singleTon().queryDataUser(userid);
    if (!vmap.isEmpty()) {
        if (vmap.value("password").toString() == password) {
            m_userId = userid;
            emit(loginRequest(m_userId ,this)); // 向server发送请求判断是不是重复登录
            return;
        }
    }
    sendObj["res"] = Fail;
    packingMessage(sendObj, Login);

}


// 返回登录结果
void ClientHandler::returnLoginRes(int restype)
{
    QList<QImage> imageList;
    QJsonObject sendObj;
    if (restype == Success) {
        m_isLogin = true;
        ChatMember* member = ChatMemberData::singleTon().getSingleMember(m_userId);
        m_headImage.load(member->getHeadPath());
        if (m_headImage.isNull()) {
            // 没有头像就返回默认头像
            m_headImage.load(":/Image/default_head.png");
            if (m_headImage.isNull()) {
                qDebug() << "用户登录获取头像失败 ";
                return;
            }
        }
        imageList.append(m_headImage);
        sendObj.insert("res", restype);
        sendObj.insert("userId", m_userId);
        packingMessage(sendObj, Login, imageList);
        sendFriendList();
        sendFriendRequestList();
        sendMessageList();
    } else if (restype == Repeat) {
        sendObj.insert("res", restype);
        packingMessage(sendObj, Login);
    }
}


// 添加好友
void ClientHandler::parseAddFriend(QJsonValue jsonvalue)
{
    if (!jsonvalue.isObject()) {
        return;
    }
    QJsonObject object = jsonvalue.toObject();
    qint64 friendId = object.value("friendId").toInteger();

    // 从内存中获取用户数据
    ChatMember* friendMember = ChatMemberData::singleTon().getSingleMember(friendId);
    QList<QImage> imageList;

    if (friendMember == nullptr) {
        // 用户不存在
        QJsonObject sendObj;
        sendObj.insert("res", Fail);
        packingMessage(sendObj, AddFriend);
    } else if (friendId == m_userId || DBManager::singleTon().queryDataFriend(m_userId, friendId)) {
        // 检查是否已经是好友或尝试添加自己
        QJsonObject sendObj;
        sendObj.insert("res", Repeat);
        packingMessage(sendObj, AddFriend);
    } else {
        // 检查是否已存在未处理的好友请求
        if (!DBManager::singleTon().queryFriendRequest(m_userId, friendId).isEmpty()) {
            QJsonObject sendObj;
            sendObj.insert("res", Repeat2);
            packingMessage(sendObj, AddFriend);
            return;
        }
        QJsonObject sendObj;
        sendObj.insert("res", Success);
        packingMessage(sendObj, AddFriend);

        ChatMember* myMember = ChatMemberData::singleTon().getSingleMember(m_userId);
        QString username = myMember->getMemberName();
        QString headpath = myMember->getHeadPath();
        QImage image(headpath);
        if (image.isNull()) {
            qDebug() << "加载图片失败......";
            return;
        }
        imageList.append(image);

        DBManager::singleTon().insertFriendRequest(m_userId, friendId);
        emit(addFriend(friendId, username, m_userId, imageList));
    }
}


// 更新头像
void ClientHandler::parseUpdateHead(qint64 userId, QList<QImage> images)
{
    if (images.isEmpty()) {
        return;
    }
    ChatMember* member = ChatMemberData::singleTon().getSingleMember(userId);
    if (member == nullptr) {
        qDebug() << "用户不存在，更新失败";
        return;
    }
    QString oldHeadPath = member->getHeadPath();
    QStringList strList = oldHeadPath.split("/");
    QString headname = strList.back();
    QImage image = images.at(0);
    QString newHeadPath = "/root/.config/OICQ/server/head/" + headname;
    if (!image.save(newHeadPath)) {
        qDebug() << "保存头像失败";
        return;
    }
    member->setHeadPath(newHeadPath);
    if (!DBManager::singleTon().updateUserHeadpath(userId, newHeadPath)) {
        qDebug() << "数据库更新头像路径失败";
        return;
    }
    QJsonObject jsonObj;
    packingMessage(jsonObj, UpdateHead, images);
}


// 处理添加好友的结果
void ClientHandler::parseAddFriendRes(QJsonValue jsonvalue)
{
    if (!jsonvalue.isObject()) {
        return;
    }
    QJsonObject obj = jsonvalue.toObject();
    int res = obj.value("res").toInt();
    qint64 friendId = obj.value("friendId").toInteger();

    if (res == Fail) {
        // 拒绝添加好友
    } else if (res == Success) {
        // 检查是否已经是好友
        ChatMember* myMember = ChatMemberData::singleTon().getSingleMember(m_userId);
        ChatMember* friendMember = ChatMemberData::singleTon().getSingleMember(friendId);
        if (myMember && friendMember && !myMember->getFriends().contains(friendMember)) {
            // 新添加好友
            myMember->addFriend(friendMember, 0);
            friendMember->addFriend(myMember, 0);
            QJsonObject sendObj;
            sendObj.insert("username", myMember->getMemberName());
            sendObj.insert("userId", m_userId);
            QList<QImage> images;
            QImage image(myMember->getHeadPath());
            if (!image.isNull()) {
                images.append(image);
            }
            emit(addFriendRes(sendObj, friendId, images));
            DBManager::singleTon().insertDataFriends(friendId, m_userId);
            DBManager::singleTon().insertDataFriends(m_userId, friendId);
        }
    }

    DBManager::singleTon().deleteFriendRequest(friendId, m_userId);
}

// 发送好友列表(从内存中获取数据)
void ClientHandler::sendFriendList()
{
    QJsonArray sendObj;
    QList<ChatMember*> friendlist = ChatMemberData::singleTon().getFriendsMap(m_userId)->keys();
    QList<QImage> imagelist;
    int i = 0;
    for (ChatMember* friend_ : friendlist) {
        qint64 fid = friend_->getAccountId();
        QString username = friend_->getMemberName();
        QString headpath = friend_->getHeadPath();
        int unreadCount = ChatMemberData::singleTon().getFriendsMap(m_userId)->value(friend_);

        QJsonObject obj;
        obj.insert("userId", fid);
        obj.insert("username", username);
        obj.insert("unreadCount", unreadCount);
        QImage image;
        image.load(headpath);
        if (image.isNull()) {
            continue;
        }

        imagelist.append(image);
        sendObj.append(obj);
        // 判断是否已经到达上限
        if (imagelist.length() == 10) {
            packingMessage(sendObj, FriendList, imagelist);
            sendObj = QJsonArray();
            imagelist.clear();
        }
    }
    if(sendObj.size() > 0){
        packingMessage(sendObj, FriendList, imagelist);
    }
}


//发送获取好友添加请求列表
void ClientHandler::sendFriendRequestList()
{
    // qDebug() << "加载离线好友请求";
    QList<QImage> imageList;
    QJsonArray jsonArray;
    QList<QVariantMap> mapLists = DBManager::singleTon().queryFriendRequests(m_userId);

    for (auto& vmap : mapLists) {
        qint64 userid = vmap.value("accountId").toLongLong();
        ChatMember* member = ChatMemberData::singleTon().getSingleMember(userid);
        if (!member) {
            qDebug() << "用户不存在，userID: " << userid;
            continue;
        }
        QJsonObject sendObj;
        sendObj.insert("username", member->getMemberName());
        sendObj.insert("userId", member->getAccountId());

        QString headpath = member->getHeadPath();
        QImage image(headpath);
        if (image.isNull()) {
            qDebug() << "error: image.isNull()  path = " << headpath;
        }
        imageList.append(image);
        jsonArray.append(sendObj);
    }

    packingMessage(jsonArray, FriendRequestList, imageList);  // 回送到客户端
}


// 发送消息列表
void ClientHandler::sendMessageList()
{
    qDebug() << "获取好友历史消息";
    QJsonArray sendArray;

    ChatMember* myMember = ChatMemberData::singleTon().getSingleMember(m_userId);
    if (!myMember) {
        qDebug() << "该用户不存在" << m_userId;
        return;
    }
    // 获取好友列表
    QMap<ChatMember*, int> friends = myMember->getFriends();
    // 遍历每个好友
    for (auto it = friends.begin(); it != friends.end(); ++it) {
        qint64 friendId = it.key()->getAccountId();
        QList<QVariantMap> sendMessageList = DBManager::singleTon().queryMessages(m_userId, friendId);
        QList<QVariantMap> recvMessageList = DBManager::singleTon().queryMessages(friendId, m_userId);
        // 合并两个列表
        QList<QVariantMap> combinedList = sendMessageList;
        combinedList.append(recvMessageList);
        // 根据日期从小到大排序
        std::sort(combinedList.begin(), combinedList.end(), [](const QVariantMap& map1, const QVariantMap& map2) {
            return map1["messageDate"].toString() < map2["messageDate"].toString();
        });
        // 遍历合并之后的消息列表
        for (const QVariantMap& messageMap : combinedList) {
            QJsonObject sendobj;
            for (QVariantMap::const_iterator it = messageMap.begin(); it != messageMap.end(); ++it) {
                sendobj.insert(it.key(), QJsonValue::fromVariant(it.value()));
            }
            sendArray.append(sendobj);
        }

        // 一次至少发送100条消息
        if (sendArray.size() >= 100) {
            packingMessage(sendArray, MessageList);
            sendArray = QJsonArray(); // 清空数据
        }
    }

    if (sendArray.size() > 0) {
        packingMessage(sendArray, MessageList);
    }
}


// 更新消息为已读
void ClientHandler::parseUpdateIsRead(QJsonValue jsonvalue)
{
    if (!jsonvalue.isObject()) {
        return;
    }
    QJsonObject obj = jsonvalue.toObject();
    qint64 friendId = obj.value("friendId").toInteger();
    ChatMemberData::singleTon().clearChatMemberUnread(m_userId, friendId);
}






