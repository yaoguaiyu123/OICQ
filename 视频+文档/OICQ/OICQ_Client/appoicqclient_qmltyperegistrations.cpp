/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#include <controller.h>
#include <framelesswindow.h>
#include <friendmodel.h>
#include <friendrequestmodel.h>
#include <messagemodel.h>
#include <screencapture.h>
#include <tcpsocket.h>
#include <textdocumenthandler.h>


#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif
Q_QMLTYPE_EXPORT void qml_register_types_oicqclient()
{
    qmlRegisterTypesAndRevisions<Controller>("oicqclient", 1);
    qmlRegisterTypesAndRevisions<FramelessWindow>("oicqclient", 1);
    qmlRegisterAnonymousType<QQuickWindow, 254>("oicqclient", 1);
    qmlRegisterAnonymousType<QWindow, 254>("oicqclient", 1);
    qmlRegisterTypesAndRevisions<FriendModel>("oicqclient", 1);
    qmlRegisterAnonymousType<QAbstractItemModel, 254>("oicqclient", 1);
    qmlRegisterTypesAndRevisions<FriendRequestModel>("oicqclient", 1);
    qmlRegisterTypesAndRevisions<MessageModel>("oicqclient", 1);
    QMetaType::fromType<QAbstractSocket *>().id();
    QMetaType::fromType<QIODevice *>().id();
    QMetaType::fromType<QTcpSocket *>().id();
    qmlRegisterTypesAndRevisions<ScreenCapture>("oicqclient", 1);
    qmlRegisterTypesAndRevisions<TcpSocket>("oicqclient", 1);
    qmlRegisterTypesAndRevisions<TextDocumentHandler>("oicqclient", 1);
    qmlRegisterModule("oicqclient", 1, 0);
}

static const QQmlModuleRegistration oicqclientRegistration("oicqclient", qml_register_types_oicqclient);
