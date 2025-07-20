#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>
#include <QList>
#include <QDebug>

class MainWindow;

/**
 * @brief 界面管理器类
 *
 * 负责管理分系统的界面显示和隐藏
 * 支持多个界面实例同时显示
 */
class UiManager : public QObject
{
    Q_OBJECT

public:
    explicit UiManager(QObject *parent = nullptr);
    ~UiManager();

    /**
     * @brief 显示新界面
     * @return 是否成功显
     */
    bool showNewWindow();

    /**
     * @brief 关闭所有界面
     */
    void closeAllWindows();

    /**
     * @brief 获取当前显示的界面数量
     * @return 界面数量
     */
    int getWindowCount() const;

    /**
     * @brief 检查是否有界面在显示
     * @return 是否有界面显示
     */
    bool hasVisibleWindows() const;

signals:
    /**
     * @brief 界面显示信号
     */
    void windowShown();

    /**
     * @brief 界面关闭信号
     */
    void windowClosed();

    /**
     * @brief 所有界面关闭信号
     */
    void allWindowsClosed();

private slots:
    /**
     * @brief 处理界面关闭事件
     */
    void handleWindowClosed();

private:
    QList<MainWindow*> m_windows;    ///< 界面列表
    int m_windowCounter;             ///< 界面计数器，用于生成标题
};

#endif // UIMANAGER_H
