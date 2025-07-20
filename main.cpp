#include "mainwindow.h"
#include "tcpserver.h"
#include "uimanager.h"
#include <QApplication>
#include <QDebug>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "=== 分系统1启动 ===";

    // 创建TCP服务端
    TcpServer *tcpServer = new TcpServer(&a);

    // 创建界面管理器
    UiManager *uiManager = new UiManager(&a);

    // 连接TCP服务端的show_ui信号到界面管理器
    MainWindow::connect(tcpServer, &TcpServer::showUiRequested, uiManager, &UiManager::showNewWindow);

    // 连接调试信号
    MainWindow::connect(tcpServer, &TcpServer::clientConnected, [](const QString &address) {
        qDebug() << "客户端连接:" << address;
    });

    MainWindow::connect(tcpServer, &TcpServer::clientDisconnected, [](const QString &address) {
        qDebug() << "客户端断开:" << address;
    });

    MainWindow::connect(tcpServer, &TcpServer::commandReceived, [](const QString &command, const QString &address) {
        qDebug() << "收到命令:" << command << "来自:" << address;
    });

    MainWindow::connect(uiManager, &UiManager::windowShown, []() {
        qDebug() << "界面已显示";
    });

    MainWindow::connect(uiManager, &UiManager::windowClosed, []() {
        qDebug() << "界面已关闭";
    });

    MainWindow::connect(uiManager, &UiManager::allWindowsClosed, []() {
        qDebug() << "所有界面已关闭";
    });

    // 启动TCP服务端
    if (!tcpServer->startServer(8080)) {
        qDebug() << "❌ TCP服务端启动失败";
        return -1;
    }

    qDebug() << "✅ TCP服务端启动成功，监听端口8080";
    qDebug() << "✅ 分系统1启动完成，等待主系统连接...";
    qDebug() << "💡 支持的命令: /show_ui, /status, /help";

    // 启动应用程序事件循环
    int result = a.exec();

    qDebug() << "=== 分系统1退出 ===";
    return result;
}
