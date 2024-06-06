#include "fileclient.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <QtConcurrent/QtConcurrentRun>
#include "global.h"
#include <QFileInfo>
#include <QEventLoop>

namespace {
qint64 maxBlock = 1024;   //一次读取的最大限制
}

FileClient::FileClient(QObject* parent)
    : QObject(parent)
    , toWrite(1)
    , haveWritten(0)
{
}

bool FileClient::connectToServer(const QString& host, quint16 port)
{
    m_socket = new QTcpSocket();
    m_socket->connectToHost(host, port);
    if (!m_socket->waitForConnected(2000)) {
        qDebug() << "连接服务端失败";
        deleteLater();
        return false;
    } else {
        connect(m_socket, &QTcpSocket::bytesWritten, this, &FileClient::handleBytesWritten);
        connect(m_socket, &QTcpSocket::disconnected, this, &FileClient::deleteLater);  //释放内存
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

    connectToServer("127.0.0.1", 8081);

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
    connectToServer("127.0.0.1", 8081);

    // // 连接接收
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
    uData.append(requestData);
    toWrite = uData.size();
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
        qDebug() << "写入到文件111 : " << haveRead;
    } else if (is_Dbegin) {
        recvFile->write(byteArray);
        haveRead += byteArray.size();
        qDebug() << "写入到文件 : " << haveRead;
    } else {
        qDebug() << "error:开头错误";
    }
    recvFile->flush();
    if (recvFile->size() >= toRead) {
        deleteLater();
    }
}



FileClient::~FileClient(){
    if(m_socket != nullptr){
        delete m_socket;
    }
}
