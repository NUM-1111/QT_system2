#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

/**
 * @brief TCP服务端类
 *
 * 负责监听主系统的连接请求，处理命令消息
 * 支持的命令：
 * - "/show_ui" : 启动前端界面
 */
class TcpServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();

    /**
     * @brief 启动TCP服务端
     * @param port 监听端口，默认8080
     * @return 启动是否成功
     */
    bool startServer(quint16 port = 8080);

    /**
     * @brief 停止TCP服务端
     */
    void stopServer();

    /**
     * @brief 检查服务端是否正在运行
     * @return 运行状态
     */
    bool isRunning() const;

    /**
     * @brief 获取当前连接的客户端数量
     * @return 客户端数量
     */
    int getClientCount() const;

signals:
    /**
     * @brief 收到show_ui命令信号
     */
    void showUiRequested();

    /**
     * @brief 客户端连接信号
     * @param clientAddress 客户端地址
     */
    void clientConnected(const QString &clientAddress);

    /**
     * @brief 客户端断开信号
     * @param clientAddress 客户端地址
     */
    void clientDisconnected(const QString &clientAddress);

    /**
     * @brief 收到命令信号
     * @param command 命令内容
     * @param clientAddress 客户端地址
     */
    void commandReceived(const QString &command, const QString &clientAddress);

private slots:
    /**
     * @brief 处理新的客户端连接
     */
    void handleNewConnection();

    /**
     * @brief 处理客户端断开连接
     */
    void handleClientDisconnected();

    /**
     * @brief 处理客户端数据
     */
    void handleClientData();

private:
    /**
     * @brief 处理接收到的命令
     * @param command 命令字符串
     * @param clientSocket 客户端socket
     */
    void processCommand(const QString &command, QTcpSocket *clientSocket);

    /**
     * @brief 发送响应给客户端
     * @param response 响应内容
     * @param clientSocket 客户端socket
     */
    void sendResponse(const QString &response, QTcpSocket *clientSocket);

    QTcpServer *m_server;           ///< TCP服务器
    QList<QTcpSocket*> m_clients;   ///< 客户端连接列表
    bool m_isRunning;               ///< 服务端运行状态
    quint16 m_port;                 ///< 监听端口
};

#endif // TCPSERVER_H
