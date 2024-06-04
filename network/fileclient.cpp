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
        qDebug() << "Connected to FileServer!";
        return true;
    }
}


void FileClient::handleBytesWritten(qint64 size)
{
    haveWritten += size;
    if (haveWritten == toWrite) {
        qDebug() << "文件传输完毕:" << toWrite;
        deleteLater(); // 释放内存
    }
}

/// 依次写入开头
/// 文件名字
/// 文件大小 8个字节
/// 文件from
/// 文件to
/// 文件包数据
void FileClient::uploadFile(const QString& filePath, qint64 from ,qint64 to)
{
    QString qtPath = filePath.mid(7);
    QtConcurrent::run([this, qtPath ,from , to]() {
        QFile file(qtPath);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray uData = QByteArray("U");
            QMetaObject::invokeMethod(this, "writeByteArray", Qt::QueuedConnection,
                Q_ARG(QByteArray, uData));
            qint64 fileOffset = 0;
            qint64 fileSize = file.size(); // 文件大小
            QByteArray infoBytes;
            QDataStream stream(&infoBytes, QIODevice::WriteOnly);
            QFileInfo fileInfo(qtPath);
            QString fileName = fileInfo.fileName(); // 获取文件名
            QByteArray namebytes = fileName.toUtf8();
            stream << fileSize; // 写入文件大小
            stream << namebytes; // 写入文件名数据
            stream << from;  // from
            stream << to; // to
            QMetaObject::invokeMethod(this, "writeByteArray", Qt::QueuedConnection,
                Q_ARG(QByteArray, infoBytes));
            qDebug() << "数据包头的长度(开头不算):    " << infoBytes.size();
            toWrite = fileSize + uData.length() + infoBytes.length(); // 初始化文件总长度
            while (fileOffset < fileSize) {
                // qDebug() << "文件传输中: " << fileOffset << "   " << fileSize;
                file.seek(fileOffset);
                QByteArray byteArray = file.read(maxBlock);
                QMetaObject::invokeMethod(this, "writeByteArray",
                    Qt::QueuedConnection, Q_ARG(QByteArray, byteArray));
                fileOffset += byteArray.size();
            }
        } else {
            qDebug() << "文件路径错误" << qtPath;
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
