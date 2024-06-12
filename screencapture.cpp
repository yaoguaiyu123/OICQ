#include "screencapture.h"
#include <QScreen>
#include <QPixmap>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

namespace {
QString printScreenDefaultPath = "/root/.config/OICQ/client/temp/screen_shot_temp.jpg";
}

ScreenCapture::ScreenCapture(QObject *parent)
    : QObject(parent)
{
    ;
}

void ScreenCapture::printScreen(const QRect &area)
{
    QScreen *primarySrceen = QGuiApplication::primaryScreen();
    QPixmap pix = primarySrceen->grabWindow(0, area.x(), area.y(), area.width(), area.height());
    pix.save(printScreenDefaultPath);
    emit screenshotComplete(); // 发出信号
}
