#include "fileclienthandler.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <QtGlobal>
#include <QDir>
#include "global.h"
#include "tcpserver.h"
#include "database/dbmanager.h"
#include <QEventLoop>

#define maxBlock 1024

FileClientHandler::FileClientHandler()
    : haveWritten(0)
    , toWrite(0)
{
    //存放到file文件夹下
    dir.setPath("/root/.config/OICQ/server");
    if (!dir.exists("file")) {
        dir.mkdir("file");
    }
}

// 处理 下载文件开头 或者 上传文件 的函数
// TODO 文件超时删除
void FileClientHandler::onReadyRead()
{
    QByteArray byteArray = m_socket->readAll();
    if (byteArray.isEmpty()) {
        return;
    }
    if (byteArray[0] == 'U' && !is_Ubegin) {
        // 上传文件
        is_Ubegin = true;
        byteArray.remove(0, 1); // 去除开头
        QDataStream stream(&byteArray, QIODevice::ReadOnly);
        stream >> totalSize; // 读取文件大小
        QByteArray nameByte;
        stream >> nameByte; // 读取文件名数据
        filename = QString::fromUtf8(nameByte);
        stream >> from;
        stream >> to;
        stream >> messageId; // 消息ID
        newFileName = dir.path() + "/file/" + getRandomString(7) + "_" + filename;
        file.setFileName(newFileName);
        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "open file fail : " << file.fileName();
            return;
        } else {
            qDebug() << "开始接收" << file.fileName() << "文件";
        }
        // 去除已读取的数据包部分
        byteArray.remove(0, sizeof(qint64) * 4 + sizeof(qint32) + nameByte.length());
        qDebug() << "将要去除的数据包的长度(开头不算):"
                 << sizeof(qint64) * 4 + sizeof(qint32) + nameByte.length();
        qDebug() << "文件名的字节数据长度:" << nameByte.length();
        file.write(byteArray);
        file.flush();
        recvSize += byteArray.size();
        qDebug() << recvSize << "   " << totalSize;
    } else if (is_Ubegin) {
        // 处理上传文件的数据包接收
        file.write(byteArray);
        file.flush();
        recvSize += byteArray.size();
        qDebug() << recvSize << "   " << totalSize;
    } else if (byteArray[0] == 'D' && !is_Dbegin) {
        // 下载文件
        is_Dbegin = true;
        byteArray.remove(0, 1);
        QDataStream stream(byteArray);
        qint64 to, from, messageId;
        stream >> messageId >> from >> to;
        // qDebug() << " " << messageId << " " << from << " " << to;
        QVariantMap vmap = DBManager::singleTon().queryFiles(messageId, from, to);
        QString filename = vmap.value("filename").toString();
        parseDownloadFile(filename);
        return;
    } else {
        // 错误
        qDebug() << "have a undefined error!";
        return;
    }

    if (is_Ubegin && file.size() >= totalSize) {
        qDebug() << "文件接收成功" << file.fileName();
        double kilobytes = totalSize / 1024.0;
        double megabytes = totalSize / 1048576.0;
        QString filesize;
        if (totalSize >= 1048576) {
            filesize = QString::number(megabytes, 'f', 2) + " MB";
        } else if (totalSize >= 1024) {
            filesize = QString::number(kilobytes, 'f', 2) + " KB";
        } else {
            filesize = QString::number(totalSize) + " B";
        }
        TcpServer::singleTon().transferFile(from, to, newFileName, filesize, messageId);
        file.close();
    }
}

// 进行下载文件的函数
/// 依次写入开头
/// 文件名字
/// 文件大小 8个字节
/// 文件包数据
void FileClientHandler::parseDownloadFile(const QString& filename)
{
    if (filename.isEmpty()) {
        qDebug() << "请求下载的文件名为空";
        return;
    }else{
        qDebug() << "要下载的文件名 : " << filename;
    }
    connect(m_socket,&QTcpSocket::bytesWritten,this,&FileClientHandler::handleBytesWritten);

    QFileInfo fileInfo(filename);
    QString fname = fileInfo.fileName();
    QFile toDownLoadfile(filename);

    if (toDownLoadfile.open(QIODevice::ReadOnly)) {
        QByteArray uData = QByteArray("D");
        m_socket->write(uData);
        qint64 fileOffset = 0;
        qint64 fileSize = toDownLoadfile.size(); // 文件大小
        toWrite = fileSize;
        QByteArray infoBytes;
        QDataStream stream(&infoBytes, QIODevice::WriteOnly);
        QByteArray namebytes = fname.toUtf8();
        stream << namebytes; // 写入文件名数据
        stream << fileSize; // 写入文件大小
        m_socket->write(infoBytes);
        while (fileOffset < fileSize) {
            toDownLoadfile.seek(fileOffset);
            QByteArray byteArray = toDownLoadfile.read(maxBlock);
            m_socket->write(byteArray);
            fileOffset += byteArray.size();
        }
    }else {
        qDebug() << "文件路径错误" << filename;
    }
    toDownLoadfile.close();
}

void FileClientHandler::handleBytesWritten(qint64 size)
{
    haveWritten += size;
    qDebug() << "成功写入到与客户端通信的socket:" << haveWritten;
    if (haveWritten == toWrite) {
        qDebug() <<"文件已经成功传送到客户端:" << toWrite;
        // deleteLater();
    }
}

void FileClientHandler::initSocket(int socketDescriptor)
{
    if (m_inited) {
        return;
    }
    m_inited = true;
    qDebug() << "文件服务器初始化Socket";
    m_socket = new QTcpSocket(this);
    if (!m_socket->setSocketDescriptor(socketDescriptor)) {
        qDebug() << "Failed to set socket descriptor";
        return;
    }

    connect(m_socket, &QTcpSocket::readyRead, this, &FileClientHandler::onReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &FileClientHandler::deleteLater);
}

FileClientHandler::~FileClientHandler()
{
    qDebug() << "文件传输完毕， 线程退出";
    if (m_socket != nullptr) {
        delete m_socket;
        m_socket = nullptr;
    }
}

