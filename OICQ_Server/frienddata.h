#pragma once

#include <QObject>

class FriendData : public QObject
{
    Q_OBJECT
public:
    explicit FriendData(QObject *parent = nullptr);

signals:
};
