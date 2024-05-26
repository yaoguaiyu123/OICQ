#include "textdocumenthandler.h"
#include <QFile>
#include <QTextBlock>
#include <QRegularExpression>
#include <QDateTime>
#include <QDir>

namespace {
//图片缓存路径
QString imageCachePath = "/root/.config/OICQ/client/send";
}

TextDocumentHandler::TextDocumentHandler(QObject* parent)
    : QObject(parent)
    , m_textDocument(nullptr)
{
}

// 插入图片
void TextDocumentHandler::insertImage(QString url, int type)
{
    if (!m_textDocument) {
        return;
    }
    QImage image(url);
    if (image.isNull()) {
        qDebug() << "图片加载失败";
        return;
    }
    int updateWidth = image.width();
    // 调整图片大小
    if (updateWidth > 200) {
        float scale = image.width() / static_cast<float>(image.height());
        updateWidth = 200;
        int updateHeight = static_cast<int>(updateWidth / scale);
        image = image.scaled(updateWidth, updateHeight, Qt::KeepAspectRatio);
    }

    // 保存图片到缓存
    QString dateFolder = QDateTime::currentDateTime().toString("yyyyMMdd");
    QDir dir(imageCachePath);
    if (!dir.exists(dateFolder)) {
        dir.mkpath(dateFolder);
    }
    QString newImagePath = dir.filePath(dateFolder + "/" + QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz") + ".jpg");
    if (!image.save(newImagePath)) {
        qDebug() << "图片保存失败";
        return;
    }

    // 插入缓存图片
    QTextDocument* doc = m_textDocument->textDocument();
    QTextCursor cursor(doc);
    cursor.setPosition(m_cursorPosition);
    QTextImageFormat imageFormat;
    imageFormat.setWidth(image.width());
    imageFormat.setHeight(image.height());
    if (type == 4) {
        imageFormat.setName("file:///" + newImagePath);
    } else if (type == 3) {
        imageFormat.setName("file://" + newImagePath);
    }
    cursor.insertImage(imageFormat);
}


void TextDocumentHandler::textContent()
{
    QTextDocument* doc = m_textDocument->textDocument();
    qDebug() << doc->toPlainText();
}

// 解析隐藏文档中的内容，将其图片内容做转化(将图片进行缩放)
void TextDocumentHandler::parseMarkDown(QString content)
{
    qDebug() << content;
    for (qint32 i = 0; i < content.length(); ++i) {
        if (content.mid(i, 9) == "![image](") {
            // 是一个图片
            i = i + 9;
            if (content.mid(i, 9) == "file:////") {
                qDebug() << "file4类型的图片";
                i = i + 9;
                QString url = "/";
                while (content[i] != ')' && i < content.length()) {
                    url += content[i];
                    ++i;
                }
                insertImage(url , 4);
                ++i;   //跳过自带的\n
            } else if (content.mid(i, 8) == "file:///") {
                qDebug() << "file3类型的图片";
                i = i + 8;
                QString url = "/";

            }
        }else{
            //不是图片
            QTextDocument* doc = m_textDocument->textDocument();
            QTextCursor cursor(doc);
            cursor.setPosition(m_cursorPosition);
            cursor.insertText(content[i]);
        }
    }
}
