#include "mainwindow.h"
#include "tcpserver.h"
#include "uimanager.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "=== 分系统2启动 ===";
    qWarning() << ">>> 警告通道测试";


    // 创建TCP服务端
    TcpServer *tcpServer = new TcpServer(&a);

    // 创建界面管理器
    UiManager *uiManager = new UiManager(&a);

    // 连接TCP服务端的 userIdReceived 信号到界面管理器
    QObject::connect(tcpServer, &TcpServer::userIdReceived,
                     uiManager, &UiManager::handleUser);

    // 连接调试信号
    QObject::connect(tcpServer, &TcpServer::clientConnected, [](const QString &address) {
        qDebug() << "客户端连接:" << address;
    });

    QObject::connect(tcpServer, &TcpServer::clientDisconnected, [](const QString &address) {
        qDebug() << "客户端断开:" << address;
    });

    QObject::connect(tcpServer, &TcpServer::commandReceived, [](const QString &command, const QString &address) {
        qDebug() << "收到命令:" << command << "来自:" << address;
    });

    QObject::connect(uiManager, &UiManager::windowClosed, []() {
        qDebug() << "窗口关闭信号接收";
    });

    // 启动TCP服务端，监听端口8080
    bool ok = tcpServer->startServer(8081);
    qDebug() << "startServer 返回:" << ok;
    if (!ok) {
        qDebug() << "错误信息:" << tcpServer->serverError();
        return -1;
    }

    qDebug() << "TCP服务端启动成功,监听8081";
    qDebug() << "分系统2启动完成! 等待主系统连接...";
    qDebug() << "支持的命令示例：userId字符串";

    // 启动应用程序事件循环
    int result = a.exec();

    qDebug() << "=== 分系统1退出 ===";
    return result;
}
