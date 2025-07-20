#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * @brief 添加日志信息
     * @param message 日志消息
     */
    void addLogMessage(const QString &message);

signals:
    /**
     * @brief 窗口关闭信号
     */
    void windowClosed();

private slots:
    /**
     * @brief 刷新状态按钮点击事件
     */
    void handleRefreshBtnClicked();

    /**
     * @brief 清空日志按钮点击事件
     */
    void handleClearLogBtnClicked();

    /**
     * @brief 关闭界面按钮点击事件
     */
    void handleCloseBtnClicked();

protected:
    /**
     * @brief 重写关闭事件
     * @param event 关闭事件
     */
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
