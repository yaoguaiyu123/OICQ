#include "screencapture.h"
#include <QScreen>
#include <QPixmap>
#include <QDir>
#include <QFileInfo>
#include "global.h"
#include <QDebug>

ScreenCapture::ScreenCapture(QObject *parent)
    : QObject(parent)
{
    ;
}

void ScreenCapture::printScreen(const QRect &area)
{
    QScreen *primarySrceen = QGuiApplication::primaryScreen();
    QPixmap pix = primarySrceen->grabWindow(0, area.x() + 2, area.y() + 2, area.width() - 5, area.height() - 5);
    pix.save(PRINT_SCREEN_DEFAULT_PATH);
    emit screenshotComplete(); // 发出信号
}


void ScreenCapture::printFullScreen()
{
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QPixmap fullScreen = primaryScreen->grabWindow(0);
    fullScreen.save(FULL_SCREEN_DEFAULT_PATH);
}
