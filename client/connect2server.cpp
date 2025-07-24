#include "connect2server.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QJsonObject>
#include <QMessageBox>
#include <QUrlQuery>

Connect2Server::Connect2Server(QWidget *parent)
    : QWidget{parent}, client(new HttpClient(this)),userId_(-1)
{
    lineEdit = new QLineEdit(this);
    button = new QPushButton("Connect", this);
    statusLabel = new QLabel("Status: Waiting...", this);
    statusLabel->setStyleSheet("QLabel { color : blue; }");
    m_Menu = new MenuWindow();
    m_Menu->hide();
    m_GomokuWidget = new GomokuWidget();
    m_GomokuWidget->hide();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(lineEdit);
    layout->addWidget(button);
    layout->addWidget(statusLabel);

    setLayout(layout);

    connect(button, &QPushButton::clicked, this, &Connect2Server::sendRequest2Connect);
    connect(client, &HttpClient::connectionSuccess, this, &Connect2Server::onRequestCompleted);
    connect(client, &HttpClient::registerSuccess, this, &Connect2Server::onRegisterSuccess);
    connect(client, &HttpClient::loginSuccess, this, &Connect2Server::onLoginSuccess);
    connect(client, &HttpClient::GameBegin, this,[this](){
        m_Menu->hide();
        m_GomokuWidget->show();
        //bool isMyTurn = isBlackFirst;后面分配黑白先手
        m_GomokuWidget->getBoard()->setMyTurn(true);
    });
    connect(m_Menu, &MenuWindow::aiGameSelected,
            this, &Connect2Server::onAIGameSelected);
    connect(m_Menu, &MenuWindow::pvpGameSelected,
            this, &Connect2Server::onPVPGameSelected);
    connect(m_GomokuWidget, &GomokuWidget::playerMoveConfirmed,
            this,  &Connect2Server::sendGomokuMoveRequest);
    connect(client, &HttpClient::receivedOpponentMove,
            this, &Connect2Server::onReceiveOpponentMove);
    connect(client, &HttpClient::PvPMatchSuccess,
            this, &Connect2Server::onPvPMatchSuccess);
    connect(client, &HttpClient::PvPWaiting,
            this, &Connect2Server::onPvPWaiting);
    connect(client, &HttpClient::PvPMoveSuccess,
            this, [this]{
        QUrl pollUrl = buildUrl("/PVP/poll");
        QUrlQuery query;
        query.addQueryItem("game_id", QString::number(m_GomokuWidget->getGameId()));
        pollUrl.setQuery(query);

        client->MovePvPPolling(pollUrl);
    });


    // connect(client, &HttpClient::receivedServerData,
    //         this, [this](const QJsonObject& json) {
    //             if (json.contains("opponentMove")) {
    //                 onReceiveOpponentMove(json["opponentMove"].toObject());
    //             }
    //         });///////////////////////这个信号没用
}

void Connect2Server::saveUrl() {
    QString text = lineEdit->text();
    if (!text.startsWith("http://") && !text.startsWith("https://")) {
        text = "http://" + text;
    }

    url = QUrl(text);
    qDebug() << "URL to connect: " << url;
}

void Connect2Server::sendRequest2Connect() {
    saveUrl();

    if (!url.isValid()) {
        statusLabel->setText("Status: Invalid URL");
        statusLabel->setStyleSheet("QLabel { color : red; }");
        return;
    }

    statusLabel->setText("Status: Connecting...");
    statusLabel->setStyleSheet("QLabel { color : orange; }");

    client->sendGetRequest(url);
}

void Connect2Server::onRequestCompleted(bool success) {
    if (success) {
        statusLabel->setText("Status: Connection successful");
        statusLabel->setStyleSheet("QLabel { color : green; }");
        // 创建登录注册界面
        if (!loginRegisterWidget) {
            loginRegisterWidget = new LoginRegisterWidget;
            // 可以连接信号处理登录或注册按钮点击
            connect(loginRegisterWidget, &LoginRegisterWidget::loginClicked, this, [this](){
                qDebug() << "登录按钮被点击，切换到登录界面";
                username = loginRegisterWidget->getUsername();
                password = loginRegisterWidget->getPassword();
                sendLoginRequest(username,password);
            });

            connect(loginRegisterWidget, &LoginRegisterWidget::registerClicked, this, [this](){
                qDebug() << "注册按钮被点击，切换到注册界面";
                username = loginRegisterWidget->getUsername();
                password = loginRegisterWidget->getPassword();

                sendRegisterRequest(username,password);
            });

        }

        this->hide();  // 隐藏当前连接界面
        loginRegisterWidget->show(); // 显示登录注册选择界面
    } else {
        statusLabel->setText("Status: Connection failed: " + client->getLastResponse().message);
        statusLabel->setStyleSheet("QLabel { color : red; }");
    }
}

void Connect2Server::sendRegisterRequest(const QString &username, const QString &password) {


    QUrl registerUrl = buildUrl("/register");

    QJsonObject payload;
    payload["username"] = username;
    payload["password"] = password;

    client->sendPostRequest(registerUrl, payload);
}



void Connect2Server::sendLoginRequest(const QString &username,
                                      const QString &password)
{

    QUrl loginUrl = buildUrl("/login");

    QJsonObject payload;
    payload["username"] = username;
    payload["password"] = password;


    client->sendPostRequest(loginUrl, payload);
}

void Connect2Server::onRegisterSuccess() {
    qDebug() << "注册成功，用户 ID:" ;
    // 可以保存用户ID，或者跳转界面等
    QMessageBox::information(this, "注册成功", "注册成功，欢迎使用！");
    // 进入菜单或主界面

}

void Connect2Server::onLoginSuccess(int ID) {
    qDebug() << "登录成功";
    QMessageBox::information(this, "登录成功", "欢迎回来！");
    // 跳转到菜单界面或主界面
    userId_ = ID;
    qDebug() << "ID是"<<userId_;
    loginRegisterWidget->hide();
    m_Menu->show();
}


QUrl Connect2Server::buildUrl(const QString &path) const {
    QUrl finalUrl = url;
    finalUrl.setPath(path);
    if (!finalUrl.scheme().startsWith("http"))
        finalUrl.setScheme("http");
    // if (finalUrl.port() == -1)
    //     finalUrl.setPort(8080);  // 设置默认端口
    return finalUrl;
}

void Connect2Server::onPVPGameSelected()
{
    isPVPing = true;
    // m_Menu->hide();
    qDebug()<<"PVP触发了槽函数";
    QUrl finalUrl = buildUrl("/PVP/start");
    client->PvPStartRequest(finalUrl);


    // m_GomokuWidget->show();
}

void Connect2Server::onAIGameSelected()
{
    isPVPing = false;
    // m_Menu->hide();
    qDebug()<<"PVP触发了槽函数";
    QUrl finalUrl = buildUrl("/aiBot/start");
    client->sendGetRequest(finalUrl);


    // m_GomokuWidget->show();
}


///////////////////////这个信号没用
void Connect2Server::onReceiveOpponentMove(int x,int y)
{
    qDebug() << "[Connect2Server] Opponent move received: row =" << x << ", col =" << y;

    m_GomokuWidget->getBoard()->confirmMove(x, y);

    // 设置我方回合
    m_GomokuWidget->getBoard()->setMyTurn(true);
    m_GomokuWidget->setOpponentMoved();
}
void Connect2Server::sendGomokuMoveRequest(int row, int col)
{
    qDebug() << "发送了网络申请";
    QJsonObject data;
    data["userId"] = userId_;
    data["x"] = row;
    data["y"] = col;
    if(isPVPing){
        data["game_id"] = m_GomokuWidget->getGameId();
        QUrl finalUrl = buildUrl("/PVP/move");
        client->PvPMoveRequest(finalUrl, data);
        m_GomokuWidget->getBoard()->setMyTurn(false);
        qDebug() << "Move sent to PVP, waiting for opponent move...";
    }
    else{
    // QUrl finalUrl = buildUrl("/pvp/move");
    QUrl finalUrl = buildUrl("/aiBot/move");

    client->sendPostRequest(finalUrl, data);
    m_GomokuWidget->getBoard()->setMyTurn(false);
    qDebug() << "Move sent to server, waiting for opponent move...";}
}

void Connect2Server::waitingForOpponent() {
    if (isWaitingCanceled)
        return;

    // 再次轮询匹配请求（每 0.5 秒）
    QTimer::singleShot(500, this, [this]() {
        client->startPvPPolling();
    });

    // 若对话框未创建，创建一个用于显示匹配中的提示框
    if (!matchingDialog) {
        matchingDialog = new QMessageBox(this);
        matchingDialog->setWindowTitle("匹配中");
        matchingDialog->setText("正在为你寻找对手，请稍候...");
        matchingDialog->setStandardButtons(QMessageBox::Cancel);
        matchingDialog->setIcon(QMessageBox::Information);
        matchingDialog->setModal(true);

        // 点击取消按钮时触发
        connect(matchingDialog, &QMessageBox::rejected, this, [this]() {
            isWaitingCanceled = true;

            // 关闭窗口
            matchingDialog->hide();
            matchingDialog->deleteLater();
            matchingDialog = nullptr;

            // TODO（可选）：向服务端发送取消排队请求
            // httpClient_->cancelMatchRequest(); // 如果你实现了的话
        });

        matchingDialog->show();
    }
}


void Connect2Server::onPvPMatchSuccess(int gameId, const QString &roleStr) {
    bool isBlack = (roleStr == "black");
    qDebug()<<roleStr;
    qDebug()<<isBlack;
    // 如果之前有对局界面，先销毁
    if (m_GomokuWidget) {
        m_GomokuWidget->close();
        m_GomokuWidget->deleteLater();
        m_GomokuWidget = nullptr;
    }

    // 创建新的游戏窗口，传入 gameId 和角色
    m_GomokuWidget = new GomokuWidget(gameId, isBlack);

    // 连接 GomokuWidget 的落子信号到网络请求发送函数
    connect(m_GomokuWidget, &GomokuWidget::playerMoveConfirmed,
            this, &Connect2Server::sendGomokuMoveRequest);


    // 连接网络对方落子信号到 GomokuWidget
    // 假设你有信号 onOpponentMoveReceived(int, int)
    connect(this, &Connect2Server::onOpponentMoveReceived,
            m_GomokuWidget, [this](int row, int col){
                // 棋盘下对方棋子
                m_GomokuWidget->getBoard()->confirmMove(row, col);
                // 切换己方回合
                m_GomokuWidget->setOpponentMoved();
            });

    // 显示对局窗口


    // 隐藏匹配等待界面
    if (matchingDialog) {
        matchingDialog->hide();
    }
    qDebug()<<"代码执行到了这里";
     m_GomokuWidget->show();
    if(!isBlack){
         QUrl pollUrl = buildUrl("/PVP/poll");
         QUrlQuery query;
         query.addQueryItem("game_id", QString::number(m_GomokuWidget->getGameId()));
         pollUrl.setQuery(query);

         client->MovePvPPolling(pollUrl);
    }
}

void Connect2Server::onPvPWaiting() {
    if (isWaitingCanceled) return; // 可选：允许取消排队

    // 间隔 500 毫秒轮询一次
    QTimer::singleShot(500, this, [this]() {
        client->startPvPPolling(); // 再次请求，函数内部使用保存的 URL
    });

    if (!matchingDialog) {
        matchingDialog = new QMessageBox(QMessageBox::Information,
                                         "匹配中", "正在为你寻找对手...", QMessageBox::Cancel, this);
        connect(matchingDialog, &QMessageBox::rejected, this, [this]() {
            isWaitingCanceled = true;
            // TODO: 向服务器发送取消排队请求（如有实现）
        });
        matchingDialog->show();
    }

}


