#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
class TcpServer;

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget* parent = nullptr);
    virtual ~Widget() override;

private:
    TcpServer* m_tcpServer;
};
#endif // WIDGET_H
