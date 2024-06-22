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


QObject* tcpSocket_qobject_singletontype_provider(QQmlEngine* engine, QJSEngine* scriptEngine)
{
    // Q_UNUSED: 抑制编译器警告
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return &TcpSocket::singleTon();
}

QObject* friendModel_qobject_singletontype_provider(QQmlEngine* engine, QJSEngine* scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return &FriendModel::singleTon();
}

QObject* friendRequestModel_qobject_singletontype_provider(QQmlEngine* engine, QJSEngine* scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return &FriendRequestModel::singleTon();
}

QObject* messageModel_qobject_singletontype_provider(QQmlEngine* engine, QJSEngine* scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return &MessageModel::singleTon();
}


int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icon/appicon.png"));   //设置应用图标
    //初始化单例类
    // std::unique_ptr<QThread> thread = std::make_unique<QThread>();
    TcpSocket &socket = TcpSocket::singleTon();
    // socket.moveToThread(thread.get());
    FriendModel::singleTon();
    FriendRequestModel::singleTon();

    qmlRegisterType<PostSend>("NetWorks", 1, 0, "PostSend"); // 注册C++类
    qmlRegisterType<ScreenCapture>("CustomWindows", 1, 0, "ScreenCapture");     // 注册C++类
    qmlRegisterType<FramelessWindow>("CustomWindows", 1, 0, "FramelessWindow"); // 注册C++类
    qmlRegisterType<TextDocumentHandler>("CustomComponents", 1, 0, "TextDocumentHandler"); // 注册C++类
    qmlRegisterType<MessageModel>("CutstomModels", 1, 0, "MessageModel"); // 注册C++类
    qmlRegisterSingletonType<TcpSocket>("NetWorks", 1, 0, "TcpSocket",
        tcpSocket_qobject_singletontype_provider);   //注册C++单例类
    qmlRegisterSingletonType<FriendModel>("CustomModels", 1, 0, "FriendModel",
        friendModel_qobject_singletontype_provider);   //注册C++单例类
    qmlRegisterSingletonType<FriendRequestModel>("CustomModels", 1, 0, "FriendRequestModel",
        friendRequestModel_qobject_singletontype_provider);   //注册C++单例类
    qmlRegisterSingletonType<MessageModel>(
        "CustomModels",
        1,
        0,
        "MessageModel",
        messageModel_qobject_singletontype_provider); //注册C++单例类
    QQmlApplicationEngine engine;
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

    // 这里要加载名称应该写 URI
    engine.loadFromModule("oicqclient", "Main");

    return app.exec();
}
