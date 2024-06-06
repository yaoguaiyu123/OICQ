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
    socket = new QTcpSocket(); //TODO
    socket->connectToHost(host, port);
    if (!socket->waitForConnected(2000)) {
        qDebug() << "连接服务端失败";
        emit complete();
        return false;
    } else {
        qDebug() << "连接文件服务器成功";
        connect(socket, &QTcpSocket::bytesWritten, this, &FileClient::handleBytesWritten);
        return true;
    }
}

void FileClient::handleBytesWritten(qint64 size)
{
    haveWritten += size;
    qDebug() << "成功写入:" << haveWritten;
    if (haveWritten == toWrite) {
        qDebug() <<"文件上传完毕:" << toWrite;
        emit complete();
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

    // if (QThread::currentThread() != socket->thread()) {
    //     return;
    // }

    QString qtPath = filePath.mid(7);
    QFile file(qtPath);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray uData = QByteArray("U");

        // QMetaObject::invokeMethod(this, "writeByteArray", Qt::QueuedConnection,
        //     Q_ARG(QByteArray, uData));
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
        // qDebug() << "文件上传的文件id" << messageId;
        // QMetaObject::invokeMethod(this, "writeByteArray", Qt::QueuedConnection,
        //     Q_ARG(QByteArray, infoBytes));
        writeByteArray(infoBytes);
        toWrite = fileSize + uData.length() + infoBytes.length(); // 初始化文件总长度
        while (fileOffset < fileSize) {
            // qDebug() << "文件传输中: " << fileOffset << "   " << fileSize;
            file.seek(fileOffset);
            QByteArray byteArray = file.read(maxBlock);
            // 写入
            // QMetaObject::invokeMethod(this, "writeByteArray", Qt::QueuedConnection,
            //     Q_ARG(QByteArray, byteArray));
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
    return socket->write(byteArray);
}

/// 依次写入开头
/// messageId
/// 文件from
/// 文件to
void FileClient::downloadFile(qint64 messageId, qint64 from, qint64 to, const QString& filepath)
{
    qDebug() << "下载文件的请求";
    connectToServer("127.0.0.1", 8081);
    qDebug() << "下载文件的请求1";

    // // 连接接收
    connect(socket, &QTcpSocket::readyRead, this, &FileClient::readDataFromServer);

    QString qtFilePath = filepath.mid(7);
    recvFile = new QFile();  //TODO
    recvFile->setFileName(qtFilePath);
    if (!recvFile->open(QIODevice::WriteOnly)) {
        qDebug() << "error:下载文件保存路径有误";
        return;
    }

    if (QThread::currentThread() != socket->thread()) {
        return;
    }

    // 发送下载文件的请求
    QByteArray uData = QByteArray("D");
    QByteArray requestData;
    QDataStream stream(&requestData, QIODevice::WriteOnly);
    stream << messageId << from << to;
    uData.append(requestData);
    writeByteArray(uData);
    qDebug() << "下载文件的请求2";
}

// 处理服务端的文件数据
void FileClient::readDataFromServer()
{
    // 最多等待30s
    if (socket->bytesAvailable() > 0) {
        QByteArray byteArray = socket->readAll();
        if (byteArray.isEmpty()) {
            qDebug() << "empty()";
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
            qDebug() << "当前接收文件大小: " << haveRead;
        } else if (is_Dbegin) {
            recvFile->write(byteArray);
            haveRead += byteArray.size();
            qDebug() << "当前接收文件大小: " << haveRead;
        } else {
            qDebug() << "error:开头错误";
        }
    }
    if (haveRead >= toRead) {
        qDebug() << "下载文件接收成功";
    } else {
        qDebug() << "下载文件接收失败";
    }
    emit(complete());
}



FileClient::~FileClient(){

}
