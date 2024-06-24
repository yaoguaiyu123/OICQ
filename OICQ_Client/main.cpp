#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "network/postsend.h"
#include "network/tcpsocket.h"
#include <QDebug>
#include "friendmodel.h"
#include "messagemodel.h"
#include <QIcon>
#include "textdocumenthandler.h"
#include "friendrequestmodel.h"
#include <memory>
#include <QThread>
#include "network/fileclient.h"
#include "framelesswindow.h"
#include <QQuickWindow>
#include "screencapture.h"
#include "messagemodel.h"
#include "controller.h"


int main(int argc, char* argv[])
{
    //初始化单例类
    // TcpSocket::singleTon();
    // FriendModel::singleTon();
    // FriendRequestModel::singleTon();
    // MessageModel::singleTon();

    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icon/appicon.png"));   //设置应用图标
    QQmlApplicationEngine engine;

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

    // 这里要加载名称应该写 URI
    engine.loadFromModule("oicqclient", "Main");

    return app.exec();
}
