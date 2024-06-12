#include "textdocumenthandler.h"
#include <QFile>
#include <QTextBlock>
#include <QRegularExpression>
#include <QDateTime>
#include <QDir>

namespace {
//图片缓存路径
QString imageCachePath = "/root/.config/OICQ/client/send";
QString printScreenDefaultPath = "/root/.config/OICQ/client/temp/screen_shot_temp.jpg";

}

TextDocumentHandler::TextDocumentHandler(QObject* parent)
    : QObject(parent)
    , m_textDocument(nullptr)
{
}

// 插入截图
void TextDocumentHandler::insertScreenshot()
{
    QString url = printScreenDefaultPath;
    qDebug() << "接收到插入截图的请求";

    if (!m_textDocument) {
        return;
    }

    QTextDocument* doc = m_textDocument->textDocument();
    QTextCursor cursor(doc);
    cursor.setPosition(m_cursorPosition); // 确保m_cursorPosition已正确初始化
    QImage image(url);


    int updateWidth = image.width();
    // 调整图片大小
    if (updateWidth > 240) {
        float scale = image.width() / static_cast<float>(image.height());
        updateWidth = 240;
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
        qDebug() << "textdocumenthandler.cpp : printScreen image fail";
        return;
    }

    // 插入缓存图片
    QTextImageFormat imageFormat;
    imageFormat.setWidth(image.width());
    imageFormat.setHeight(image.height());
    imageFormat.setName("file://" + newImagePath);
    cursor.insertImage(imageFormat);
}


// 插入图片
void TextDocumentHandler::insertImage(QString url)
{
    if (!m_textDocument) {
        return;
    }
    QImage image(url);
    if (image.isNull()) {
        //TODO 说明这是一段文字,要insert文字
        qDebug() << "textdocumenthandler.cpp : paste image load fail";
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
        qDebug() << "textdocumenthandler.cpp : paste image fail";
        return;
    }

    // 插入缓存图片
    QTextDocument* doc = m_textDocument->textDocument();
    QTextCursor cursor(doc);
    cursor.setPosition(m_cursorPosition);
    QTextImageFormat imageFormat;
    imageFormat.setWidth(image.width());
    imageFormat.setHeight(image.height());
    imageFormat.setName("file://" + newImagePath);   // 插入file3类型的图片
    cursor.insertImage(imageFormat);
}

void TextDocumentHandler::insertText(QString content){
    QTextDocument* doc = m_textDocument->textDocument();
    QTextCursor cursor(doc);
    cursor.setPosition(m_cursorPosition);
    cursor.insertText(content);
}

void TextDocumentHandler::textContent()
{
    QTextDocument* doc = m_textDocument->textDocument();
    qDebug() << doc->toPlainText();
}

// 解析隐藏文档中的内容，将其图片内容做转化(将图片进行缩放)
void TextDocumentHandler::parseMarkDown(QString content)
{
    for (qint32 i = 0; i < content.length(); ++i) {
        // qDebug() << content.mid(i, 8);
        if (content.mid(i, 9) == STR_IMAGE_TYPE) {
            // 是一个图片(可识别的图片)
            i = i + 9;
            if (content.mid(i, 9) == FILE4_TYPE) {
                i = i + 9;
                QString url = "/";
                while (content[i] != ')' && i < content.length()) {
                    url += content[i];
                    ++i;
                }
                insertImage(url);
                ++i;   //跳过自带的\n
            }else if(content.mid(i, 8) == FILE3_TYPE){
                i = i + 8;
                QString url = "/";
                while (content[i] != ')' && i < content.length()) {
                    url += content[i];
                    ++i;
                }
                insertImage(url);
                ++i;
            }

        } else if (content.mid(i, 8) == FILE3_TYPE) {
            // 是一个文件
            qDebug() << "是一个文件";
            i = i + 8;
            QString url = "/";
            while (content[i] != '\n' && i < content.length()) {
                url += content[i];
                ++i;
            }
            // 如果是图片文件就插入
            QImage image(url);
            if (!image.isNull()) {
                insertImage(url);
            } else {
                insertText("file://" + url);
            }
            ++i;
        }else{
            // 普通文字
            QTextDocument* doc = m_textDocument->textDocument();
            QTextCursor cursor(doc);
            cursor.setPosition(m_cursorPosition);
            cursor.insertText(content[i]);
        }
    }
}
