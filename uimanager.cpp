#include "uimanager.h"
#include <QDebug>

UiManager::UiManager(QObject *parent)
    : QObject(parent)
{
}

UiManager::~UiManager()
{
    // 程序退出前关闭所有窗口
    for (auto window : m_userWindows) {
        if (window) {
            window->close();
            window->deleteLater();
        }
    }
    m_userWindows.clear();
}

void UiManager::handleUser(const QString &userId)
{
    if (m_userWindows.contains(userId)) {
        MainWindow *window = m_userWindows[userId];
        if (window->isHidden()) {
            window->show();
            window->raise();
            window->activateWindow();
            qDebug() << "恢复显示窗口:" << userId;
        } else {
            qDebug() << "窗口已存在并可见:" << userId;
        }
    } else {
        // 创建新的窗口
        MainWindow *newWindow = new MainWindow;
        newWindow->setWindowTitle("用户窗口 - " + userId);

        // 记录映射关系
        m_userWindows[userId] = newWindow;

        // 绑定关闭信号，用于隐藏或移除
        connect(newWindow, &MainWindow::windowClosed,
                this, &UiManager::handleWindowClosed);

        newWindow->show();
        qDebug() << "创建并显示新窗口:" << userId;
    }
}

void UiManager::handleWindowClosed()
{
    MainWindow *window = qobject_cast<MainWindow*>(sender());
    if (!window) return;

    // 查找属于哪个用户ID
    QString userIdToRemove;
    for (auto it = m_userWindows.begin(); it != m_userWindows.end(); ++it) {
        if (it.value() == window) {
            userIdToRemove = it.key();
            break;
        }
    }

    if (!userIdToRemove.isEmpty()) {
        qDebug() << "窗口已关闭（隐藏）:" << userIdToRemove;
        emit windowClosed();
        // 不移除映射，仅隐藏
        window->hide();
    }

}
