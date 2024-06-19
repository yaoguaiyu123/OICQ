#include "widget.h"
#include "tcpserver.h"

Widget::Widget(QWidget* parent)
    : QWidget(parent)
{
    m_tcpServer = new TcpServer(this);
    resize(400, 400);
}

Widget::~Widget()
{
}
