#include "global.h"
#include <QRandomGenerator>

//将QByteArray转换为QJsonValue的函数
QJsonValue byteArrayToJson(const QByteArray& jsonData) {
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isNull()) {
        if (doc.isObject()) {
            return doc.object();
        }else if (doc.isArray()){
            return doc.array();
        }else {
            qDebug() << "Json Document 无效";
        }
    } else {
        qDebug() << "无效的JSON:" << jsonData;
    }
    return QJsonValue();
}

//jsonobject 转换为 QString
QString JsonObjectToString(const QJsonObject &jsonObject) {
    QJsonDocument doc(jsonObject);
    QString jsonString = doc.toJson(QJsonDocument::Indented); // 使用Indented参数可以让输出的JSON字符串是格式化的，易于阅读
    return jsonString;
}


QString generateRandomAlphanumeric(int length) {
    QString alphanumeric = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    QString randomString;
    for (int i = 0; i < length; ++i) {
        int index = QRandomGenerator::global()->generate() % alphanumeric.length();
        randomString += alphanumeric.at(index);
    }
    return randomString;
}
