#include "tcpsocket.h"
#include <QDebug>
#include <QtEndian>
#include "../global.h"
#include <QBuffer>
namespace{
QString ServerIP = "127.0.0.1";
int PORT = 8080;
}

TcpSocket::TcpSocket(QObject* parent)
    : QTcpSocket { parent } ,
    m_hostAddress(new QHostAddress())
{
    connect(this, &TcpSocket::readyRead, this, &TcpSocket::on_ready_read);
    m_hostAddress->setAddress(ServerIP);
    m_sendbuf = new uchar[16 * 1024 * 1024];
}


TcpSocket& TcpSocket::singleTon()
{
    static TcpSocket tcpsocket;
    return tcpsocket;
}

// 将信息打包成为一个MESG对象
void TcpSocket::packingMessage(QString value, int msgType, QList<QImage> imageList)
{
    qDebug() << "打包数据" << value;
    QJsonDocument doc = QJsonDocument::fromJson(value.toUtf8());
    if (doc.isNull()) {
        qDebug() << "JSON解析失败";
        return;
    }

    QByteArray bytes = doc.toJson();
    qDebug() << "打包的JSON数据大小:" << bytes.size();

    MESG* message = new MESG();
    message->msg_type = static_cast<MSG_TYPE>(msgType);
    message->data = new uchar[bytes.size()];
    memcpy(message->data, bytes.data(), bytes.size());
    message->len = bytes.size();

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
    sendMessage(message);
}

// 将信息打包成为一个MESG对象
void TcpSocket::packingMessageFromJson(QJsonValue value, int msgType, QList<QImage> imageList)
{
    QJsonDocument doc(value.toObject());
    if (doc.isNull()) {
        qDebug() << "JSON解析失败";
        return;
    }

    QByteArray bytes = doc.toJson();
    qDebug() << "打包的JSON数据大小:" << bytes.size();

    MESG* message = new MESG();
    message->msg_type = static_cast<MSG_TYPE>(msgType);
    message->data = new uchar[bytes.size()];
    memcpy(message->data, bytes.data(), bytes.size());
    message->len = bytes.size();

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
    sendMessage(message);
}

int TcpSocket::getUserId()
{
    return m_userId;
}

//连接服务器
void TcpSocket::connectToServer(){
    connectToHost(*m_hostAddress,PORT);
}

//发送数据的函数
/**
 1个字节的开头 $
 2个字节的 msg_type(大端格式)
 8个字节的 userid(大端格式)
 4个字节的 len(大端格式)
 len个字节的 data
 1个字节的结尾 #
*/
void TcpSocket::sendMessage(MESG* send)
{
    if (state() == QAbstractSocket::UnconnectedState) {
        if (send->data)
            free(send->data);
        if (send)
            free(send);
        return;
    }

    quint64 bytestowrite = 0;
    m_sendbuf[bytestowrite++] = '$'; // 开头
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
    for (const QByteArray& imgData : send->imagesData) {
        qToBigEndian<quint32>(imgData.size(), m_sendbuf + bytestowrite);
        bytestowrite += 4;
        memcpy(m_sendbuf + bytestowrite, imgData.constData(), imgData.size());
        bytestowrite += imgData.size();
    }

    m_sendbuf[bytestowrite++] = '#'; // 结尾

    qDebug() << "写入:" << bytestowrite << " " << send->len;
    qint64 hastowrite = bytestowrite;
    qint64 ret = 0, haswrite = 0;
    do {
        ret = write((char*)m_sendbuf + haswrite, hastowrite - haswrite);
        waitForBytesWritten();
        if (ret == -1 && error() == QAbstractSocket::TemporaryError) {
            ret = 0;
        } else if (ret == -1) {
            break;
        }
        haswrite += ret;
    } while (haswrite < hastowrite); // 确保所有数据都被写入


    if (send->data) {
        free(send->data);
    }
    if (send) {
        free(send);
    }
    qDebug() << "发送数据结束";
}


// 接收数据的函数
void TcpSocket::on_ready_read()
{
    qDebug() << "开始读取数据";

    while (bytesAvailable() > 0) {
        int aliasSize = bytesAvailable();
        QByteArray datagram;
        datagram.resize(aliasSize);
        read(datagram.data(), aliasSize); // 读取数据

        qDebug() << aliasSize << "    这是我接收的数据的总数";

        if (datagram.at(0) == '$') {
            // 读到开头
            m_recvbuf.append(datagram);   //append会自动扩容
            // qDebug() << "读到开头";
        } else {
            m_recvbuf.append(datagram);
        }

        if (datagram.at(aliasSize - 1) == '#') {
            // qDebug() << "读到结尾";
            break;
        } else {
            return;
        }
    }

    //解析数据包
    int pos = 0;
    char beginC = m_recvbuf.at(pos++);
    if (beginC != '$') {
        qDebug() << "开头错误 " << beginC;
        return;
    }
    quint16 msgType = qFromBigEndian<quint16>(reinterpret_cast<const uchar*>(m_recvbuf.constData() + pos));
    pos += sizeof(quint16); // 消息类型
    quint64 userId = qFromBigEndian<quint64>(reinterpret_cast<const uchar*>(m_recvbuf.constData() + pos));
    pos += sizeof(quint64); // id
    quint32 msgLen = qFromBigEndian<quint32>(reinterpret_cast<const uchar*>(m_recvbuf.constData() + pos));
    pos += sizeof(quint32); // json长度
    QByteArray msgData = m_recvbuf.mid(pos, msgLen);
    pos += msgLen; // json
    quint32 imgCount = qFromBigEndian<quint32>(reinterpret_cast<const uchar*>(m_recvbuf.constData() + pos));
    pos += sizeof(quint32); // 读取图片数量
    // 读取图片存储到images
    QList<QImage> images;
    for (quint32 i = 0; i < imgCount; ++i) {
        quint32 imgSize = qFromBigEndian<quint32>(reinterpret_cast<const uchar*>(m_recvbuf.constData() + pos));
        pos += sizeof(quint32);
        QByteArray imgData = m_recvbuf.mid(pos, imgSize);
        pos += imgSize;
        QImage image;
        if (image.loadFromData(imgData)) {
            images.append(image);
        }
    }

    if (m_recvbuf.at(pos) != '#') {
        qDebug() << "数据包结尾错误";
        return;
    }


    switch (msgType) {
    case PrivateMessage:
        parsePrivateMessage(byteArrayToJson(msgData), images);
        break;
    case GroupMessage:
        break;
    case AddFriend:
        parseAddFriend(byteArrayToJson(msgData));
        break;
    case Login:
        parseLogin(byteArrayToJson(msgData), images);
        break;
    case FriendList:
        parseFriendList(byteArrayToJson(msgData), images);
        break;
    case UpdateHead:
        parseUpdateHead(images);
        break;
    case AddFriendRequest:
        parseAddFriendRequest(byteArrayToJson(msgData),images);
        break;
    case AddFriendRes:
        parseAddFriendRes(byteArrayToJson(msgData), images);
        break;
    case FriendRequestList:
        parseFriendRequestList(byteArrayToJson(msgData), images);
        break;
    default:
        break;
    }

    m_recvbuf.clear();  //清空缓冲区
}



void TcpSocket::parsePrivateMessage(QJsonValue jsonvalue,QList<QImage>& images)
{
    // qDebug() << "私发消息!!!!!!!!!!!!!!!!!图片的数量: " << images.count();
    emit privateMessageReturn(jsonvalue,images);
}

// 处理添加好友请求
void TcpSocket::parseAddFriend(QJsonValue jsonvalue)
{
    if (!jsonvalue.isObject()) {
        return;
    }
    QJsonObject jsonObj = jsonvalue.toObject();
    int res = jsonObj.value("res").toInt();
    emit(addFriend(res));
}

void TcpSocket::parseLogin(QJsonValue jsonvalue,QList<QImage>& images)
{
    if (!jsonvalue.isObject()) {
        return;
    }
    QJsonObject object = jsonvalue.toObject();
    qDebug() << "parseLogin::登录返回 " << object.value("res").toInt();
    emit loginReturn(object.value("res").toInt(),images.first()); // 发出信号
    emit loginReturnToQml(object.value("res").toInt());
    if (object.value("res").toInt() == Success) {
        qDebug() << "parseLogin::登录成功";
        m_userId = object.value("userId").toInteger();
        packingMessage("{}",FriendList);   //发送好友列表的请求
    } else if (object.value("res").toInt() == Fail) {
        //TODO
        qDebug() << "登录失败";
    }
}

void TcpSocket::parseFriendList(QJsonValue jsonvalue,QList<QImage>& images)
{
    // qDebug() << "接收到好友列表" << jsonvalue;
    if (!jsonvalue.isArray()) {
        return;
    }
    emit friendListReturn(jsonvalue ,images);
    packingMessage("{}",FriendRequestList);  //获取好友请求列表
}

void TcpSocket::parseUpdateHead(QList<QImage>& images)
{
    emit updateHeadReturn(images.at(0));
}

// 处理添加好友请求
void TcpSocket::parseAddFriendRequest(QJsonValue jsonvalue,QList<QImage> & images)
{
    if (!jsonvalue.isObject()) {
        return;
    }
    QJsonObject object = jsonvalue.toObject();
    QString fromname = object.value("fromname").toString();
    qint64 fromid = object.value("fromid").toInteger();
    emit(addFriendRequest(fromname,fromid, images));
}

//处理添加好友的结果(添加一个好友)
void TcpSocket::parseAddFriendRes(QJsonValue jsonvalue, QList<QImage>& images)
{
    emit(addFriendRes(jsonvalue,images));
}

// 处理添加好友的请求列表
void TcpSocket::parseFriendRequestList(QJsonValue jsonvalue, QList<QImage>& images)
{
    if (!jsonvalue.isObject()) {
        return;
    }
    QJsonObject object = jsonvalue.toObject();
    QString fromname = object.value("username").toString();
    qint64 fromid = object.value("userid").toInteger();
    emit(addFriendRequest(fromname,fromid, images));
}











