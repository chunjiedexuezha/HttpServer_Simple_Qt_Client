#ifndef CONNECT2SERVER_H
#define CONNECT2SERVER_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include "httpclient.h"
#include "loginregisterwidget.h"
#include "menuwindow.h"
#include "gomokuwidget.h"  // 添加头文件

class Connect2Server : public QWidget
{
    Q_OBJECT
public:
    explicit Connect2Server(QWidget *parent = nullptr);
    void sendLoginRequest();
    QUrl buildUrl(const QString &path)const;
    void waitingForOpponent();

private slots:
    void sendRequest2Connect();
    void onRequestCompleted(bool success);
    void sendLoginRequest(const QString &username,
                          const QString &password);
    void sendRegisterRequest(const QString &username,
                        const QString &password);
    void onRegisterSuccess();
    void onLoginSuccess(int ID);
    void onPVPGameSelected();
    void onAIGameSelected();
private slots:
    void onPvPMatchSuccess(int gameId, const QString &roleStr);
    void onPvPWaiting();
public slots:
    void sendGomokuMoveRequest(int row, int col);


    // 处理服务器对对手落子数据的响应（示范，实际视你的设计而定）
    void onReceiveOpponentMove(int x,int y);

signals:
    void opponentMoved(int row, int col);
    void onOpponentMoveReceived(int row, int col);

private:
    void saveUrl();

    QLineEdit *lineEdit;
    QPushButton *button;
    QLabel *statusLabel;
    QUrl url;

    HttpClient *client;  // 作为成员变量
    LoginRegisterWidget *loginRegisterWidget = nullptr;
    QString username;
    QString password;
    MenuWindow* m_Menu;
     GomokuWidget *m_GomokuWidget = nullptr;  // 初始化为空
     int userId_; // 记录登录后用户ID
     int pendingRow_; // 上一次准备落子的行
     int pendingCol_; // 上一次准备落子的列
     bool blackTurn_ = true; // 当前是否轮到黑棋
    // 用于弹出匹配等待的对话框
    QPointer<QMessageBox> matchingDialog = nullptr;

     // 标志是否取消排队
     bool isWaitingCanceled = false;
    bool  isPVPing;
};

#endif // CONNECT2SERVER_H
