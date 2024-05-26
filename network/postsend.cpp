#include "postsend.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

namespace {
QString getRes(QByteArray& dataArray)
{
    QJsonParseError error;
    // 首先获得QJsonDocument对象
    QJsonDocument jsonDoc = QJsonDocument::fromJson(dataArray, &error);
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "解析失败0 " << error.errorString();
    }
    if (!jsonDoc.isObject()) {
        qDebug() << "解析失败1";
    }
    QJsonObject outObject = jsonDoc.object();
    QString result = outObject.value("result").toString();
    return result;
}
}

PostSend::PostSend(QObject* parent)
    : QObject(parent)
    , networkManager(new QNetworkAccessManager(this))
{
}

void PostSend::sendPostRequest(const QUrl& url, const QString ques,int id)
{
    // 构建JSON对象
    QJsonArray jsonArray;
    QJsonObject messageObject;
    messageObject["role"] = "user";
    messageObject["content"] = ques;
    jsonArray.append(messageObject);
    QJsonObject jsonObject;
    jsonObject["messages"] = jsonArray;

    // 创建json文档
    QJsonDocument doc(jsonObject);
    // 将json文档转化为QByteArray
    QByteArray postData = doc.toJson();

    // 创建一个请求
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");


    // 发送请求
    if (m_reply != nullptr) {
        delete m_reply; // 释放之前的reply
        m_reply = nullptr;
    }
    m_reply= networkManager->post(request, postData);

    connect(m_reply, &QNetworkReply::finished, [this,id]() {
        QString strRes;
        if (m_reply->error() == QNetworkReply::NoError) {
            // 成功
            QByteArray responseData = m_reply->readAll();
            strRes = getRes(responseData);
        } else {
            // 错误
            strRes = "error";
            qDebug() << "post请求失败: " << m_reply->errorString();
        }
        emit this->replyFinished(strRes,id);
    });
}

PostSend::~PostSend()
{
    if (m_reply != nullptr) {
        delete m_reply;
        m_reply = nullptr;
    }
}

