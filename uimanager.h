#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>
#include <QMap>
#include <QString>
#include "mainwindow.h"

class UiManager : public QObject
{
    Q_OBJECT
public:
    explicit UiManager(QObject *parent = nullptr);
    ~UiManager();

signals:
    void windowClosed();

public slots:
    void handleUser(const QString &userId); // 显示或恢复窗口

private slots:
    void handleWindowClosed(); // 捕捉窗口关闭信号

private:
    QMap<QString, MainWindow*> m_userWindows; // 用户ID → 窗口映射
};

#endif // UIMANAGER_H
