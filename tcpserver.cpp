#include "tcpserver.h"
#include <QNetworkInterface>

TcpServer::TcpServer(QObject *parent)
    : QObject(parent)
    , m_server(nullptr)
    , m_isRunning(false)
    , m_port(8080)
{
}

TcpServer::~TcpServer()
{
    stopServer();
}

bool TcpServer::startServer(quint16 port)
{
    if (m_isRunning) {
        qDebug() << "TCP服务端已经在运行";
        return true;
    }

    m_port = port;
    m_server = new QTcpServer(this);

    // 连接信号和槽
    connect(m_server, &QTcpServer::newConnection, this, &TcpServer::handleNewConnection);

    // 启动监听
    if (!m_server->listen(QHostAddress::Any, m_port)) {
        qDebug() << "TCP服务端启动失败:" << m_server->errorString();
        return false;
    }

    m_isRunning = true;
    qDebug() << "TCP服务端启动成功，监听端口:" << m_port;

    // 输出本机IP地址，便于主系统连接
    QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
    for (const QHostAddress &address : addresses) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol &&
            address != QHostAddress::LocalHost) {
            qDebug() << "本机IP地址:" << address.toString() << ":" << m_port;
        }
    }

    return true;
}

void TcpServer::stopServer()
{
    if (!m_isRunning) {
        return;
    }

    // 关闭所有客户端连接
    for (QTcpSocket *client : m_clients) {
        client->disconnectFromHost();
        client->deleteLater();
    }
    m_clients.clear();

    // 停止服务器
    if (m_server) {
        m_server->close();
        m_server->deleteLater();
        m_server = nullptr;
    }

    m_isRunning = false;
    qDebug() << "TCP服务端已停止";
}

bool TcpServer::isRunning() const
{
    return m_isRunning;
}

int TcpServer::getClientCount() const
{
    return m_clients.size();
}

void TcpServer::handleNewConnection()
{
    QTcpSocket *clientSocket = m_server->nextPendingConnection();
    if (!clientSocket) {
        return;
    }

    // 连接客户端信号
    connect(clientSocket, &QTcpSocket::readyRead, this, &TcpServer::handleClientData);
    connect(clientSocket, &QTcpSocket::disconnected, this, &TcpServer::handleClientDisconnected);

    // 添加到客户端列表
    m_clients.append(clientSocket);

    QString clientAddress = clientSocket->peerAddress().toString() + ":" +
                            QString::number(clientSocket->peerPort());

    qDebug() << "新客户端连接:" << clientAddress;
    emit clientConnected(clientAddress);

    // 发送欢迎消息
    sendResponse("欢迎连接到分系统1！", clientSocket);
}

void TcpServer::handleClientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) {
        return;
    }

    QString clientAddress = clientSocket->peerAddress().toString() + ":" +
                            QString::number(clientSocket->peerPort());

    // 从客户端列表中移除
    m_clients.removeOne(clientSocket);
    clientSocket->deleteLater();

    qDebug() << "客户端断开连接:" << clientAddress;
    emit clientDisconnected(clientAddress);
}

void TcpServer::handleClientData()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) {
        return;
    }

    // 读取数据
    QByteArray data = clientSocket->readAll();
    QString command = QString::fromUtf8(data).trimmed();

    QString clientAddress = clientSocket->peerAddress().toString() + ":" +
                            QString::number(clientSocket->peerPort());

    qDebug() << "收到来自" << clientAddress << "的命令:" << command;
    emit commandReceived(command, clientAddress);

    // 处理命令
    processCommand(command, clientSocket);
}

void TcpServer::processCommand(const QString &command, QTcpSocket *clientSocket)
{
    QString response;

    if (command == "/show_ui") {
        qDebug() << "收到show_ui命令，准备启动界面";
        emit showUiRequested();
        response = "OK: 界面启动命令已执行";
    }
    else if (command == "/status") {
        response = QString("OK: 分系统1运行正常，当前连接数: %1").arg(m_clients.size());
    }
    else if (command == "/help") {
        response = "支持的命令:\n"
                   "/show_ui - 启动前端界面\n"
                   "/status - 查看系统状态\n"
                   "/help - 显示帮助信息";
    }
    else {
        response = QString("ERROR: 未知命令 '%1'，输入 /help 查看支持的命令").arg(command);
    }

    sendResponse(response, clientSocket);
}

void TcpServer::sendResponse(const QString &response, QTcpSocket *clientSocket)
{
    if (!clientSocket || clientSocket->state() != QAbstractSocket::ConnectedState) {
        return;
    }

    QByteArray data = response.toUtf8();
    clientSocket->write(data);

    QString clientAddress = clientSocket->peerAddress().toString() + ":" +
                            QString::number(clientSocket->peerPort());
    qDebug() << "发送响应给" << clientAddress << ":" << response;
}
