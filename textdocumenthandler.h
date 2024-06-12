#ifndef TEXTDOCUMENTHANDLER_H
#define TEXTDOCUMENTHANDLER_H

// 该类注册为一个普通类

#include <QQuickTextDocument>
#include <QTextDocument>
#include <QTextImageFormat>
#include <QTextCursor>
#include "global.h"
#include <QCursor>

class TextDocumentHandler : public QObject {
    Q_OBJECT
    Q_PROPERTY_AUTO(QQuickTextDocument*,hideTextDocument)
    Q_PROPERTY_AUTO(QQuickTextDocument*,textDocument)
    Q_PROPERTY_AUTO(int, cursorPosition)  //当前光标位置
public:
    TextDocumentHandler(QObject* parent = nullptr);
    Q_INVOKABLE void insertImage(QString);
    Q_INVOKABLE void insertScreenshot();
    void insertText(QString);
    Q_INVOKABLE void textContent();
    Q_INVOKABLE void parseMarkDown(QString content);

private:
};



#endif // TEXTDOCUMENTHANDLER_H
