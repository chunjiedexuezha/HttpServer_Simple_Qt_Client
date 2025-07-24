#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPointer>
#include <QTimer>
#include <QUrl>

struct ResponseData {
    int httpStatus = 0;
    QByteArray rawData;
    QJsonObject jsonData;
    QString message;
    bool success = false;
};


class HttpClient : public QObject
{
    Q_OBJECT

public:
    explicit HttpClient(QObject *parent = nullptr);

    // 发送GET请求
    void sendGetRequest(const QUrl &url);

    // 发送POST请求，附带JSON数据
    void sendPostRequest(const QUrl &url, const QJsonObject &payload);

    const ResponseData& getLastResponse() const { return lastResponse; }
    void sendPostRequest22(const QUrl &url);

    void PvPStartRequest(const QUrl &url);              // 发起匹配请求
    void startPvPPolling();//轮询函数

    void PvPMoveRequest(const QUrl &url,const QJsonObject &payload);              // 发起移动
    void MovePvPPolling(const QUrl &url);//轮询函数
    void MovePvPPolling();//轮询函数

signals:
    void requestCompleted(bool success);
    void registerSuccess();
    void loginSuccess(int ID);
    void connectionSuccess(bool success);
    void genericSuccess(const QString &message);
    void GameBegin();
    void receivedOpponentMove(int row, int col);//游戏移动

    void registerFailed(const QString &message);
    void loginFailed(const QString &message);
    void genericFailed(const QString &message);

    void PvPMatchSuccess(int gameId,const QString &roleStr);
    void PvPWaiting();//正在等待
    void PvPMoveSuccess();//落子成功

private:
    void setupReplyHandling(QPointer<QNetworkReply> reply);
    void handleJsonReply(QPointer<QNetworkReply> reply);
    void PvPStartReplyHandling(QPointer<QNetworkReply> reply);  // 处理匹配回复
    void PvPMoveReplyHandling(QPointer<QNetworkReply> reply);  // 处理移动



    QNetworkAccessManager *manager;
    ResponseData lastResponse;
    int userId_ = -1;
    int _gameId = -1;
    QUrl pvpMatchUrl_;
    QUrl pvpMoveUrl_;
    QUrl pvpPollMoveUrl_;
    QTimer* pollingTimer_ = nullptr;
    bool isWaiting_ = false; // 用于是否正在匹配

};

#endif // HTTPCLIENT_H
