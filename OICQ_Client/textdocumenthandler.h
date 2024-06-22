#ifndef TEXTDOCUMENTHANDLER_H
#define TEXTDOCUMENTHANDLER_H

// 该类注册为一个普通类

#include <QQuickTextDocument>
#include <QTextDocument>
#include <QTextImageFormat>
#include <QTextCursor>
#include "global.h"
#include <QCursor>
#include <QtQml/qqmlregistration.h>

class TextDocumentHandler : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(TextDocumentHandler)
    Q_PROPERTY_AUTO(QQuickTextDocument*,hideTextDocument)
    Q_PROPERTY_AUTO(QQuickTextDocument*,textDocument)
    Q_PROPERTY_AUTO(int, cursorPosition)  //当前光标位置
public:
    TextDocumentHandler(QObject* parent = nullptr);
    void insertImage(QString);
    void insertEmoji(QString, int width = -1,int height = -1);
    void insertText(QString);
    QString textContent();
    Q_INVOKABLE void insertScreenshot();
    Q_INVOKABLE bool isTextContentEmpty();
    Q_INVOKABLE void parseHtml();
    Q_INVOKABLE void parseHtmlWithEmoji(int type);


private:
};



#endif // TEXTDOCUMENTHANDLER_H
