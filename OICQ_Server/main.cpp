#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QNetworkInterface>
#include <QThread>
#include "database/dbmanager.h"
#include "fileserver.h"
#include "tcpserver.h"
#include "widget.h"


namespace {
void printLocalIPs()
{
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QNetworkInterface &networkInterface : QNetworkInterface::allInterfaces()) {
        for (const QNetworkAddressEntry &entry : networkInterface.addressEntries()) {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol && entry.ip() != localhost) {
                qDebug() << "我的IP地址: " << entry.ip().toString();
            }
        }
    }
}
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    printLocalIPs();
    TcpServer::singleTon();
    FileServer fileserver;
    return a.exec();
}
