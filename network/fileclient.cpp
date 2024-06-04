#include "fileclient.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <QtConcurrent/QtConcurrentRun>
#include "global.h"
#include <QFileInfo>

namespace {
qint64 maxBlock = 4096;   //一次读取的最大限制
}

FileClient::FileClient(QObject* parent)
    : QObject(parent)
    , toWrite(0)
    , haveWritten(0)
{
    connect(&socket, &QTcpSocket::bytesWritten, this, &FileClient::handleBytesWritten);
}

bool FileClient::connectToServer(const QString& host, quint16 port)
{
    socket.connectToHost(host, port);
    if (!socket.waitForConnected(2000)) {
        qDebug() << "连接服务端失败";
        return false;
    } else {
        qDebug() << "Connected to server!";
        return true;
    }
}


void FileClient::handleBytesWritten(qint64 size)
{
    haveWritten += size;
    if (haveWritten == toWrite) {
        qDebug() << "确认已经写入完毕";
        deleteLater(); // 释放内存
    }
}

/// 依次写入开头
/// 文件名字长度 4个字节
/// 文件名字
/// 文件大小 8个字节
/// 文件包数据
void FileClient::uploadFile(const QString& filePath)
{

    QtConcurrent::run([this, filePath]() {
        QFile file(filePath);

        if (file.open(QIODevice::ReadOnly)) {
            QByteArray uData = QByteArray("U");
            QMetaObject::invokeMethod(this, "writeByteArray", Qt::QueuedConnection,
                Q_ARG(QByteArray, uData));
            qint64 fileOffset = 0;
            qint64 fileSize = file.size(); // 文件大小
            toWrite = fileSize + 1 + sizeof(qint64);
            QByteArray sizeBytes;
            QDataStream stream(&sizeBytes, QIODevice::WriteOnly);
            stream << fileSize;   //写入文件大小
            QMetaObject::invokeMethod(this, "writeByteArray", Qt::QueuedConnection,
                Q_ARG(QByteArray, sizeBytes));
            while (fileOffset < fileSize) {
                // qDebug() << fileOffset << "   " << fileSize;
                // 同步调用
                file.seek(fileOffset);
                QByteArray byteArray = file.read(maxBlock);
                QMetaObject::invokeMethod(this, "writeByteArray",
                    Qt::DirectConnection, Q_ARG(QByteArray, byteArray));
                fileOffset += byteArray.size();
            }
        } else {
            qDebug() << "文件路径错误";
        }
        file.close();
    });
}


qint64 FileClient::writeByteArray(const QByteArray& byteArray)
{
    return socket.write(byteArray);
}



FileClient::~FileClient(){
}
