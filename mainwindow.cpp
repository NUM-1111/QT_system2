#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置窗口标题
    setWindowTitle("分系统1");

    // 连接按钮信号
    connect(ui->refreshBtn, &QPushButton::clicked, this, &MainWindow::handleRefreshBtnClicked);
    connect(ui->clearLogBtn, &QPushButton::clicked, this, &MainWindow::handleClearLogBtnClicked);
    connect(ui->closeBtn, &QPushButton::clicked, this, &MainWindow::handleCloseBtnClicked);

    // 添加启动日志
    addLogMessage("分系统1界面已创建");
    addLogMessage("等待主系统命令...");

    qDebug() << "分系统1界面已创建";
}

MainWindow::~MainWindow()
{
    delete ui;
    qDebug() << "分系统1界面已销毁";
}

void MainWindow::addLogMessage(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString logMessage = QString("[%1] %2").arg(timestamp, message);

    ui->logTextEdit->append(logMessage);

    // 滚动到底部
    QTextCursor cursor = ui->logTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->logTextEdit->setTextCursor(cursor);
}

void MainWindow::handleRefreshBtnClicked()
{
    addLogMessage("刷新状态按钮被点击");
    ui->statusLabel->setText("状态：已刷新 - " + QDateTime::currentDateTime().toString("hh:mm:ss"));
}

void MainWindow::handleClearLogBtnClicked()
{
    ui->logTextEdit->clear();
    addLogMessage("日志已清空");
}

void MainWindow::handleCloseBtnClicked()
{
    addLogMessage("关闭界面按钮被点击");
    close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "分系统1界面关闭事件触发";

    // 发出窗口关闭信号
    emit windowClosed();

    // 接受关闭事件
    event->accept();

    // 注意：这里不调用父类的closeEvent，因为我们只是隐藏窗口，不退出程序
    // 窗口的实际销毁由UiManager处理
}
