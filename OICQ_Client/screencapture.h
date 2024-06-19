#ifndef SCREENCAPTURE_H
#define SCREENCAPTURE_H

// 该类用于捕获屏幕

#include <QObject>
#include <QPixmap>

class ScreenCapture : public QObject
{
    Q_OBJECT
public:
    explicit ScreenCapture(QObject *parent = nullptr);

    Q_INVOKABLE void printScreen(const QRect &area);
    Q_INVOKABLE void printFullScreen();
signals:
    void screenshotComplete(); //截图完毕信号
};

#endif // SCREENCAPTURE_H
