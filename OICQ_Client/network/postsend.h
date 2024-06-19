#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class PostSend : public QObject {
    Q_OBJECT
public:
    explicit PostSend(QObject* parent = nullptr);
    ~PostSend();
signals:
    void postRequestCompleted(QNetworkReply* reply);

public slots:
    // Q_INVOKABLE void sendPostRequest(const QUrl &url, const QByteArray &data);
    //不一定要Q_INVOKABLE，直接声明为slots也行
    void sendPostRequest(const QUrl& url, const QString ques,int id = 0);

signals:
    void replyFinished(QString,int);

private:
    QNetworkAccessManager *networkManager;
    QNetworkReply* m_reply = nullptr;
};

#endif // NETWORKMANAGER_H

