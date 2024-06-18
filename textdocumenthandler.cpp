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

// 插入截图
void TextDocumentHandler::insertScreenshot()
{
    if (!m_textDocument) {
        return;
    }

    QTextDocument* doc = m_textDocument->textDocument();
    QTextCursor cursor(doc);
    cursor.setPosition(m_cursorPosition); // 确保m_cursorPosition已正确初始化
    QImage image(PRINT_SCREEN_DEFAULT_PATH);

    // 保存图片到缓存
    QString dateFolder = QDateTime::currentDateTime().toString("yyyyMMdd");
    QDir dir(imageCachePath);
    if (!dir.exists(dateFolder)) {
        dir.mkpath(dateFolder);
    }
    QString newImagePath = dir.filePath(dateFolder + "/" + QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz") + ".jpg");
    if (!image.save(newImagePath)) {
        qDebug() << "textdocumenthandler.cpp : printScreen image fail  " << newImagePath;
        return;
    }

    // 插入缓存图片
    QTextImageFormat imageFormat;
    if(image.width() > 200){
        imageFormat.setWidth(200);
    }else{
        imageFormat.setWidth(image.width());
    }
    imageFormat.setName("file://" + newImagePath);
    cursor.insertImage(imageFormat);
}


// 插入图片
void TextDocumentHandler::insertImage(QString strurl)
{
    if (!m_textDocument) {
        return;
    }
    bool isQrc = false;
    if (strurl.startsWith("qrc")) {
        strurl.remove(0, 3);
        isQrc = true;
    }
    QUrl url(strurl);
    QImage image;
    if (isQrc) {
        image.load(strurl);
    } else {
        image.load(url.toLocalFile());
    }
    if (image.isNull()) {
        insertText(strurl);
        qDebug() << "textdocumenthandler.cpp : paste image load fail, url = " << url.toLocalFile();
        return;
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

    if(image.width() > 280){
        imageFormat.setWidth(280);
    }else{
        imageFormat.setWidth(image.width());
    }
    imageFormat.setName("file:///" + newImagePath);   // 插入file4类型的图片
    cursor.insertImage(imageFormat);
}

void TextDocumentHandler::insertText(QString content){
    QTextDocument* doc = m_textDocument->textDocument();
    QTextCursor cursor(doc);
    cursor.setPosition(m_cursorPosition);
    cursor.insertText(content);
}

QString TextDocumentHandler::textContent()
{
    QTextDocument* doc = m_textDocument->textDocument();
    return doc->toPlainText();
}

bool TextDocumentHandler::isTextContentEmpty()
{
    return textContent().isEmpty();
}



// 解析隐藏文档中的内容，将其图片内容做转化(将图片进行缩放)
void TextDocumentHandler::parseHtml()
{
    QTextDocument* doc = m_hideTextDocument->textDocument(); //获取隐藏文档
    QTextBlock block = doc->begin(); // 获取文档的第一个块

    while (block.isValid()) {
        QTextBlock::iterator it;
        // 遍历块中的所有元素
        for (it = block.begin(); !(it.atEnd()); ++it) {
            QTextFragment fragment = it.fragment();
            if (fragment.isValid()) {
                if (fragment.charFormat().isImageFormat()) {
                    // 处理图片
                    QTextImageFormat imgFormat = fragment.charFormat().toImageFormat();
                    QString imgName = imgFormat.name(); // 图片名称
                    insertImage(imgName);
                } else {
                    // 处理文字
                    QString content = fragment.text();
                    int startIndex = 0;
                    int lastEndIndex = 0;
                    // 寻找是否有非标准格式的图片
                    while ((startIndex = content.indexOf("file://", lastEndIndex)) != -1) {
                        if (startIndex > lastEndIndex) {
                            insertText(
                                content.mid(lastEndIndex, startIndex - lastEndIndex).trimmed());
                        }
                        int endIndex = content.indexOf(' ', startIndex);
                        if (endIndex == -1) {
                            endIndex = content.length();
                        }

                        QString filePath = content.mid(startIndex, endIndex - startIndex);
                        if (filePath.endsWith(".png") || filePath.endsWith(".jpg")
                            || filePath.endsWith(".jpeg")) {
                            insertImage(filePath);
                        } else {
                            insertText(filePath); // 路径不是图片，插入普通文本
                        }
                        lastEndIndex = endIndex;
                    }
                    // 寻找是否有qrc图片
                    while ((startIndex = content.indexOf("qrc:/", lastEndIndex)) != -1) {
                        if (startIndex > lastEndIndex) {
                            insertText(content.mid(lastEndIndex, startIndex - lastEndIndex).trimmed());
                        }
                        int endIndex = content.indexOf(' ', startIndex);
                        if (endIndex == -1) {
                            endIndex = content.length();
                        }

                        QString filePath = content.mid(startIndex, endIndex - startIndex);
                        if (filePath.endsWith(".png") || filePath.endsWith(".jpg")
                            || filePath.endsWith(".jpeg")) {
                            insertImage(filePath);
                        } else {
                            insertText(filePath); // 路径不是图片，插入普通文本
                        }
                        lastEndIndex = endIndex;
                    }




                    if (lastEndIndex < content.length()) {
                        insertText(content.mid(lastEndIndex).trimmed());
                    }
                }
            }
        }
        block = block.next(); // 移动到下一个块
    }
}
