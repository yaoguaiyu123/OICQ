#include "global.h"
#include <QDateTime>

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


QString getRandomString(int nLen)
{
    srand(QDateTime::currentMSecsSinceEpoch());
    const char ch[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int size = sizeof(ch);
    char* str = new char[nLen + 1];
    int num = 0;
    for (int nIndex = 0; nIndex < nLen; ++nIndex)
    {
        num = rand() % (size - 1);
        str[nIndex] = ch[num];
    }
    str[nLen] = '\0';
    QString res(str);
    return res;
}
