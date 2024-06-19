#include "fileclient.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <QtConcurrent/QtConcurrentRun>
#include "global.h"
#include <QFileInfo>
#include <QEventLoop>
#include <QTimer>

namespace {
qint64 maxBlock = 1024;   //一次读取的最大限制
}

FileClient::FileClient(int friendIndex ,int messageIndex, QObject* parent)
    : QObject(parent)
    , haveRead(0)
    , toRead(9999)
    , toWrite(9999)
    , haveWritten(0)
    , m_index(friendIndex)
    , m_messageIndex(messageIndex)
{
}

bool FileClient::connectToServer(const QString& host, quint16 port)
{
    m_used = true;
    m_socket = new QTcpSocket();
    m_socket->connectToHost(host, port);
    m_timer = new QTimer;
    m_timer->start(500);
    connect(m_timer, &QTimer::timeout, this, &FileClient::handlerTimeout);
    if (!m_socket->waitForConnected(2000)) {
        qDebug() << "连接服务端失败";
        deleteLater();
        return false;
    } else {
        connect(m_socket, &QTcpSocket::bytesWritten, this, &FileClient::handleBytesWritten);
        connect(m_socket, &QTcpSocket::disconnected, this, &FileClient::deleteLater);  //释放内存
        connect(this, &FileClient::aboutToCancel, this, &FileClient::deleteLater);
        return true;
    }
}

// 处理文件上传的网络套接字写入
void FileClient::handleBytesWritten(qint64 size)
{
    haveWritten += size;
    qDebug() << "成功写入到socket:" << haveWritten << " "  << toWrite;
    if (haveWritten == toWrite) {
        qDebug() <<"文件上传完毕:" << toWrite;
        emit updateFileMessage(m_index,m_messageIndex, haveWritten, toWrite);
        // deleteLater();
        m_socket->disconnectFromHost();
    }
}

/// 依次写入开头
/// 文件名字
/// 文件大小 8个字节
/// 文件from
/// 文件to
/// 消息id
/// 文件包数据
void FileClient::uploadFile(const QString& filePath, qint64 from ,qint64 to,qint64 messageId)
{
    if (m_used) {
        return;
    }
    upOrDown = true;
    connectToServer(IPADDRESS, 8081);
    QString qtPath = filePath.mid(7);
    QFile file(qtPath);

    if (file.open(QIODevice::ReadOnly)) {
        QByteArray uData = QByteArray("U");
        writeByteArray(uData);
        qint64 fileOffset = 0;
        qint64 fileSize = file.size(); // 文件大小
        QByteArray infoBytes;
        QDataStream stream(&infoBytes, QIODevice::WriteOnly);
        QFileInfo fileInfo(qtPath);
        QString fileName = fileInfo.fileName(); // 获取文件名
        QByteArray namebytes = fileName.toUtf8();
        stream << fileSize; // 写入文件大小
        stream << namebytes; // 写入文件名数据
        stream << from; // from
        stream << to; // to
        stream << messageId; // messageid
        writeByteArray(infoBytes);
        toWrite = fileSize + uData.length() + infoBytes.length(); // 初始化文件总长度
        while (fileOffset < fileSize) {
            file.seek(fileOffset);
            QByteArray byteArray = file.read(maxBlock);
            writeByteArray(byteArray);
            fileOffset += byteArray.size();
        }
    } else {
        qDebug() << "文件路径错误" << qtPath;
    }
    file.close();
}

qint64 FileClient::writeByteArray(const QByteArray& byteArray)
{
    return m_socket->write(byteArray);
}

/// 依次写入开头
/// messageId
/// 文件from
/// 文件to
void FileClient::downloadFile(qint64 messageId, qint64 from, qint64 to, const QString& filepath)
{
    if (m_used) {
        return;
    }
    upOrDown = false;
    connectToServer(IPADDRESS, 8081);

    // 连接接收
    connect(m_socket, &QTcpSocket::readyRead, this, &FileClient::readDataFromServer);

    QString qtFilePath = filepath.mid(7);
    recvFile = new QFile();  //TODO
    recvFile->setFileName(qtFilePath);
    if (!recvFile->open(QIODevice::WriteOnly)) {
        qDebug() << "error:下载文件保存路径有误";
        return;
    }
    // qDebug() << messageId << " " << from << " " << to << " " << filepath;

    // 发送下载文件的请求
    QByteArray uData = QByteArray("D");
    QByteArray requestData;
    QDataStream stream(&requestData, QIODevice::WriteOnly);
    stream << messageId << from << to;
    qDebug() << "messageId:  " << messageId << "你好世界";
    uData.append(requestData);
    // toWrite = uData.size();
    writeByteArray(uData);
}

// 处理服务端的文件数据
void FileClient::readDataFromServer()
{
    QByteArray byteArray = m_socket->readAll();
    if (byteArray.isEmpty()) {
        return;
    }

    if (byteArray[0] == 'D' && !is_Dbegin) {
        byteArray.remove(0, 1); // 去除开头
        is_Dbegin = true;
        QDataStream stream(byteArray);
        QByteArray nameByte;
        stream >> nameByte; // 读取文件名数据
        recvfilename = QString::fromUtf8(nameByte);
        stream >> toRead;
        byteArray.remove(0, sizeof(qint64) + sizeof(qint32) + recvfilename.length());
        recvFile->write(byteArray);
        haveRead += byteArray.size();
        // qDebug() << "写入到文件111 : " << haveRead;
    } else if (is_Dbegin) {
        recvFile->write(byteArray);
        haveRead += byteArray.size();
        // qDebug() << "写入到文件 : " << haveRead;
    } else {
        qDebug() << "error:开头错误";
    }
    recvFile->flush();
    if (recvFile->size() >= toRead) {
        emit updateFileMessage(m_index,m_messageIndex, haveRead, toRead);
        // deleteLater();
        m_socket->disconnectFromHost();
    }
}


void FileClient::handlerTimeout()
{
    if (upOrDown == true) {
        qDebug() << "上传文件的进度  " << haveWritten << " " << toWrite;
        emit updateFileMessage(m_index,m_messageIndex, haveWritten, toWrite);
    } else {
        // 下载文件的进度
        qDebug() << "下载文件进度 " << haveRead << " " << toRead;
        emit updateFileMessage(m_index,m_messageIndex, haveRead, toRead);
    }
}

int FileClient::getIndex()
{
    return m_index;
}

int FileClient::getMessageIndex()
{
    return m_messageIndex;
}

void FileClient::cancelTransfer()
{
    emit(aboutToCancel());
}


FileClient::~FileClient(){
    qDebug() << "文件传输完毕， 文件传输线程退出......";
    if (m_timer != nullptr) {
        m_timer->stop();
        delete m_timer;
    }
    if(m_socket != nullptr){
        if(m_socket->state() == QAbstractSocket::ConnectedState){
            m_socket->disconnectFromHost();
        }
        delete m_socket;
    }
}
