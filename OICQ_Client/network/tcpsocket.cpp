#include "tcpsocket.h"
#include <QDebug>
#include <QtEndian>
#include "../global.h"
#include <QBuffer>
namespace{
int PORT = 8080;
}


TcpSocket::TcpSocket(QObject* parent)
    : QTcpSocket { parent } ,
    m_hostAddress(new QHostAddress())
{
    connect(this, &TcpSocket::readyRead, this, &TcpSocket::on_readyRead);
    m_hostAddress->setAddress(IPADDRESS);
    m_sendbuf = new uchar[32 * 1024 * 1024];
}


TcpSocket& TcpSocket::singleTon()
{
    static TcpSocket tcpsocket(nullptr);
    return tcpsocket;
}

// 将信息打包成为一个MESG对象
void TcpSocket::packingMessageFromJson(const QJsonValue &value, int msgType, QList<QImage> imageList)
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
 4个字节的包大小(大端格式)
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

    quint32 bytestowrite = 0;
    m_sendbuf[bytestowrite++] = '$'; // 开头
    bytestowrite += 4;  //为包大小预留空间
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
    qToBigEndian<quint32>(bytestowrite, m_sendbuf + 1);   //包大小



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
void TcpSocket::on_readyRead()
{
    qDebug() << "开始读取数据";
    while (bytesAvailable() > 0) {
        int aliasSize = bytesAvailable();
        QByteArray datagram;
        datagram.resize(aliasSize);
        read(datagram.data(), aliasSize); // 读取数据

        qDebug() << aliasSize << "    这是我接收的数据的总数";
        m_recvbuf.append(reinterpret_cast<const uchar*>(datagram.data()), aliasSize);
        parsePackage(); // 解析包
    }
}

// 对包进行解析
void TcpSocket::parsePackage()
{
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
    // qDebug() << "缓冲区剩余数据" << m_recvbuf.availableSpaceSize();
    QJsonValue jsonData = byteArrayToJson(msgData);
    switch (msgType) {
    case PrivateMessage:
        parsePrivateMessage(jsonData, images);
        break;
    case FileMessage:
        perseFileMessage(jsonData);
        break;
    case AddFriend:
        parseAddFriend(jsonData);
        break;
    case Login:
        parseLogin(jsonData, images);
        break;
    case FriendList:
        parseFriendList(jsonData, images);
        break;
    case UpdateHead:
        parseUpdateHead(images);
        break;
    case AddFriendRequest:
        parseAddFriendRequest(jsonData,images);
        break;
    case AddFriendRes:
        parseAddFriendRes(jsonData, images);
        break;
    case FriendRequestList:
        parseFriendRequestList(jsonData, images);
        break;
    case MessageList:
        parseHistoryMessageList(jsonData);
        break;
    default:
        break;
    }
    if(m_recvbuf.availableSpaceSize() > 0){
        parsePackage();  //递归调用
    }
}


void TcpSocket::parsePrivateMessage(const QJsonValue& jsonvalue,const QList<QImage>& images)
{
    // qDebug() << "私发消息!!!!!!!!!!!!!!!!!图片的数量: " << images.count();
    emit privateMessageReturn(jsonvalue,images);
}

// 处理添加好友请求
void TcpSocket::parseAddFriend(const QJsonValue& jsonvalue)
{
    if (!jsonvalue.isObject()) {
        return;
    }
    QJsonObject jsonObj = jsonvalue.toObject();
    int res = jsonObj.value("res").toInt();
    emit(addFriend(res));
}

// 处理登录的返回
void TcpSocket::parseLogin(const QJsonValue& jsonvalue,const QList<QImage>& images)
{
    if (!jsonvalue.isObject()) {
        return;
    }
    QJsonObject object = jsonvalue.toObject();
    if (object.value("res").toInt() == Success) {
        emit loginReturn(images.first()); // 发出信号
        qDebug() << "登录成功，准备发送信号给FriendModel     ###############################";
        m_userId = object.value("userId").toInteger();
    }
    emit loginReturnToQml(object.value("res").toInt());
}

// 处理文件消息
void TcpSocket::perseFileMessage(const QJsonValue& jsonvalue)
{
    emit fileMessage(jsonvalue);
}

void TcpSocket::parseFriendList(const QJsonValue& jsonvalue,const QList<QImage>& images)
{
    // qDebug() << "接收到好友列表" << jsonvalue;
    if (!jsonvalue.isArray()) {
        return;
    }
    emit friendListReturn(jsonvalue ,images);
}

void TcpSocket::parseUpdateHead(const QList<QImage>& images)
{
    emit updateHeadReturn(images.at(0));
}

// 处理添加好友请求(添加一个好友)
void TcpSocket::parseAddFriendRequest(const QJsonValue& jsonvalue,const QList<QImage> & images)
{
    if (!jsonvalue.isObject()) {
        return;
    }
    emit(addFriendSingleRequest(jsonvalue, images));
}

//处理添加好友的结果
void TcpSocket::parseAddFriendRes(const QJsonValue& jsonvalue,const QList<QImage>& images)
{
    emit(addFriendRes(jsonvalue,images));
}

// 处理添加好友的请求列表(登录时获取)
void TcpSocket::parseFriendRequestList(const QJsonValue& jsonvalue,const QList<QImage>& images)
{
    if (!jsonvalue.isArray()) {
        return;
    }
    emit(addFriendRequest(jsonvalue, images));
}


// 处理好友消息的返回
void TcpSocket::parseHistoryMessageList(const QJsonValue& jsonvalue)
{
    if (!jsonvalue.isArray()) {
        return;
    }
    emit(historyMessageList(jsonvalue));
}








