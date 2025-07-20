#include "uimanager.h"
#include "mainwindow.h"

UiManager::UiManager(QObject *parent)
    : QObject(parent)
    , m_windowCounter(0)
{
}

UiManager::~UiManager()
{
    closeAllWindows();
}

bool UiManager::showNewWindow()
{
    // 创建新的主窗口
    MainWindow *window = new MainWindow();
    if (!window) {
        qDebug() << "创建界面失败";
        return false;
    }

    // 设置窗口标题
    m_windowCounter++;
    QString title = QString("分系统1 - 界面 %1").arg(m_windowCounter);
    window->setWindowTitle(title);

    // 连接窗口关闭信号
    connect(window, &MainWindow::windowClosed, this, &UiManager::handleWindowClosed);

    // 添加到窗口列表
    m_windows.append(window);

    // 显示窗口
    window->show();
    window->raise();
    window->activateWindow();

    qDebug() << "显示新界面:" << title << "，当前界面数量:" << m_windows.size();
    emit windowShown();

    return true;
}

void UiManager::closeAllWindows()
{
    qDebug() << "关闭所有界面，当前界面数量:" << m_windows.size();

    // 关闭所有窗口
    for (MainWindow *window : m_windows) {
        if (window) {
            window->close();
            window->deleteLater();
        }
    }

    m_windows.clear();
    m_windowCounter = 0;

    emit allWindowsClosed();
    qDebug() << "所有界面已关闭";
}

int UiManager::getWindowCount() const
{
    return m_windows.size();
}

bool UiManager::hasVisibleWindows() const
{
    for (MainWindow *window : m_windows) {
        if (window && window->isVisible()) {
            return true;
        }
    }
    return false;
}

void UiManager::handleWindowClosed()
{
    MainWindow *window = qobject_cast<MainWindow*>(sender());
    if (!window) {
        return;
    }

    // 从列表中移除
    m_windows.removeOne(window);
    window->deleteLater();

    qDebug() << "界面已关闭，当前界面数量:" << m_windows.size();
    emit windowClosed();

    // 如果没有界面了，发出信号
    if (m_windows.isEmpty()) {
        qDebug() << "所有界面已关闭";
        emit allWindowsClosed();
    }
}
