#include "httpclient.h"
#include <QDebug>

HttpClient::HttpClient(QObject *parent)
    : QObject(parent), manager(new QNetworkAccessManager(this)) {}

void HttpClient::sendGetRequest(const QUrl &url) {
    lastResponse = ResponseData();
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QPointer<QNetworkReply> reply = manager->get(request);
    qDebug() << "get发送了一条消息"<<url;
    setupReplyHandling(reply);
}

void HttpClient::sendPostRequest(const QUrl &url, const QJsonObject &payload)
{
    lastResponse = ResponseData();

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QByteArray data = QJsonDocument(payload).toJson(QJsonDocument::Compact);

    // ========= 调试信息 =========
    qDebug() << "\n=========== HTTP POST 调试信息 ===========";
    qDebug() << "POST URL:" << url.toString();
    qDebug() << "POST Data:" << data;
    qDebug() << "Actual Data Length:" << data.size();

    // 不手动设置 Content-Length，让 Qt 自动完成（避免出错）

    // 启动请求
    QPointer<QNetworkReply> reply = manager->post(request, data);

    // 错误处理
    connect(reply, &QNetworkReply::errorOccurred, this, [reply]() {
        qDebug() << "请求错误:" << reply->errorString();
    });

    // 响应处理
   setupReplyHandling(reply);
}


void HttpClient::PvPStartRequest(const QUrl &url) {
    pvpMatchUrl_ = url;

    // 首次发请求
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QPointer<QNetworkReply> reply = manager->get(request);
    qDebug() << "[HttpClient] 首次PvP匹配请求发送: " << url;

    PvPStartReplyHandling(reply);
}

void HttpClient::startPvPPolling() {
    if (pvpMatchUrl_.isEmpty()) return;
    PvPStartRequest(pvpMatchUrl_);
}

void HttpClient::PvPMoveRequest(const QUrl &url, const QJsonObject &payload) {
    pvpMoveUrl_ = url;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QByteArray data = QJsonDocument(payload).toJson(QJsonDocument::Compact);


    QPointer<QNetworkReply> reply = manager->post(request,data);
    qDebug() << "[HttpClient] 首次PvP匹配请求发送: " << url;

    PvPMoveReplyHandling(reply);
}

void HttpClient::MovePvPPolling(const QUrl &url) {
    pvpPollMoveUrl_ = url;
    QNetworkRequest request((QUrl(url)));
    // request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QPointer<QNetworkReply> reply = manager->get(request);
    PvPMoveReplyHandling(reply);
}
void HttpClient::MovePvPPolling() {

    QNetworkRequest request((QUrl(pvpPollMoveUrl_)));
    // request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QPointer<QNetworkReply> reply = manager->get(request);
    PvPMoveReplyHandling(reply);
}


void HttpClient::setupReplyHandling(QPointer<QNetworkReply> reply) {
    if (!reply) return;

    // 设置 5 秒超时保护
    QTimer *timeoutTimer = new QTimer(this);
    timeoutTimer->setSingleShot(true);
    timeoutTimer->start(5000);

    connect(timeoutTimer, &QTimer::timeout, this, [=]() {
        if (reply && reply->isRunning()) {
            reply->abort();
            lastResponse.message = "Request timed out";
            emit requestCompleted(false);
        }
        if (reply) reply->deleteLater();
        timeoutTimer->deleteLater();
    });

    connect(reply, &QNetworkReply::finished, this, [=]() {
        timeoutTimer->stop();
        timeoutTimer->deleteLater();
        qDebug() << "收到一条消息: ";
        if (!reply) {
            lastResponse.message = "Reply pointer was null";
            qDebug() << "消息不存在: ";
            emit requestCompleted(false);
            return;
        }

        lastResponse.httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        lastResponse.rawData = reply->readAll();

        if (reply->error() != QNetworkReply::NoError) {
            lastResponse.message = "Network error: " + reply->errorString();
             qDebug() << "有错误: "<<reply->errorString();
            emit requestCompleted(false);
            reply->deleteLater();
            return;
        }

        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(lastResponse.rawData, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            lastResponse.message = "JSON parse error: " + parseError.errorString();
            qDebug() << "解析错误: ";
            emit requestCompleted(false);
            reply->deleteLater();
            return;
        }

        lastResponse.jsonData = jsonDoc.object();
        lastResponse.message = lastResponse.jsonData["message"].toString();
        QString statusStr = lastResponse.jsonData["status"].toString().toLower();
        qDebug() << "message: " << lastResponse.message;
        qDebug() << "statusStr: " << statusStr;
        qDebug() << "状态: " << lastResponse.httpStatus;

        lastResponse.success = (lastResponse.httpStatus == 200);

        if (lastResponse.success) {

            if (statusStr == "success" || statusStr == "ok"||lastResponse.jsonData["success"].toBool()) {
                if (lastResponse.message == "Register successful") {
                    emit registerSuccess();
                } else if (lastResponse.message == "Login successful") {
                    userId_ = lastResponse.jsonData["userId"].toInt();
                    qDebug() << "ID: " << userId_;
                    emit loginSuccess(userId_);
                } else if (lastResponse.message == QString::fromUtf8("连接成功")) {
                    emit connectionSuccess(true);
                    qDebug() << "触发连接成功: " ;
                }else if (lastResponse.message == "AI game started") {//游戏开始
                    emit GameBegin();
                    qDebug() << "游戏开始: " ;
                }else if (lastResponse.message == "Move_Back") {//游戏回复

                    qDebug() << "收到移动回复: " ;
                    QJsonObject lastMoveObj = lastResponse.jsonData["last_move"].toObject();
                    if (lastMoveObj.contains("x") && lastMoveObj.contains("y")) {
                        int x = lastMoveObj["x"].toInt();
                        int y = lastMoveObj["y"].toInt();

                        qDebug() << "[HttpClient] Opponent move parsed: x =" << x << ", y =" << y;

                        // 发出信号，通知 Connect2Server
                        emit receivedOpponentMove(x, y);}

                }
                else {
                    emit genericSuccess(lastResponse.message);
                }
            } else {


                emit genericFailed(lastResponse.message);
            }
        } else {
            qDebug() << "状态码不是200: " ;
            qDebug() << "message: " << lastResponse.message;
            if (lastResponse.message == "username already exists") {
                emit registerFailed(lastResponse.message);
            } else if (lastResponse.message == "Invalid username or password") {
                emit loginFailed(lastResponse.message);
            } else {
                emit genericFailed(lastResponse.message);
            }
        }
        emit requestCompleted(lastResponse.success);
        reply->deleteLater();
    });
}

void HttpClient::PvPStartReplyHandling(QPointer<QNetworkReply> reply) {
    if (!reply) return;

    QTimer* timeoutTimer = new QTimer(this);
    timeoutTimer->setSingleShot(true);
    timeoutTimer->start(5000);

    connect(timeoutTimer, &QTimer::timeout, this, [=]() {
        if (reply && reply->isRunning()) {
            reply->abort();
            lastResponse.message = "Request timed out";
            emit requestCompleted(false);
        }
        if (reply) reply->deleteLater();
        timeoutTimer->deleteLater();
    });

    connect(reply, &QNetworkReply::finished, this, [=]() {
        timeoutTimer->stop();
        timeoutTimer->deleteLater();

        lastResponse.httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        lastResponse.rawData = reply->readAll();

        if (reply->error() != QNetworkReply::NoError) {
            lastResponse.message = "Network error: " + reply->errorString();
            emit requestCompleted(false);
            reply->deleteLater();
            return;
        }

        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(lastResponse.rawData, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            lastResponse.message = "JSON parse error: " + parseError.errorString();
            emit requestCompleted(false);
            reply->deleteLater();
            return;
        }

        lastResponse.jsonData = jsonDoc.object();
        lastResponse.message = lastResponse.jsonData["message"].toString();
        lastResponse.success = (lastResponse.httpStatus == 200 &&
                                (lastResponse.jsonData["status"].toString() == "ok" ||
                                 lastResponse.jsonData["success"].toBool()));

        if (lastResponse.success) {
            if (lastResponse.message == "match_success") {
                isWaiting_ = false;
                if (pollingTimer_) pollingTimer_->stop();

                _gameId = lastResponse.jsonData["game_id"].toInt();
                QString roleStr = lastResponse.jsonData["role"].toString();


                emit PvPMatchSuccess(_gameId, roleStr);
                return;
            }
            else if (lastResponse.message == "waiting_for_opponent") {
                // 继续等待，由轮询控制，不处理
                emit PvPWaiting();
                return;
            } else {
                emit genericSuccess(lastResponse.message);
            }
        } else {
            emit genericFailed(lastResponse.message);
        }

        emit requestCompleted(lastResponse.success);
        reply->deleteLater();
    });
}

void HttpClient::PvPMoveReplyHandling(QPointer<QNetworkReply> reply) {
    if (!reply) return;

    QTimer* timeoutTimer = new QTimer(this);
    timeoutTimer->setSingleShot(true);
    timeoutTimer->start(5000);  // 超时 5 秒

    connect(timeoutTimer, &QTimer::timeout, this, [=]() {
        if (reply && reply->isRunning()) {
            reply->abort();
            lastResponse.message = "Request timed out";
            // emit PvPMoveFailed("请求超时");
        }
        if (reply) reply->deleteLater();
        timeoutTimer->deleteLater();
    });

    connect(reply, &QNetworkReply::finished, this, [=]() {
        timeoutTimer->stop();
        timeoutTimer->deleteLater();

        lastResponse.httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        lastResponse.rawData = reply->readAll();

        if (reply->error() != QNetworkReply::NoError) {
            lastResponse.message = "Network error: " + reply->errorString();
            // emit PvPMoveFailed(lastResponse.message);
            reply->deleteLater();
            return;
        }

        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(lastResponse.rawData, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            lastResponse.message = "JSON parse error: " + parseError.errorString();
            // emit PvPMoveFailed(lastResponse.message);
            reply->deleteLater();
            return;
        }

        lastResponse.jsonData = jsonDoc.object();
        lastResponse.message = lastResponse.jsonData["message"].toString();
        lastResponse.success = (lastResponse.httpStatus == 200 &&
                                (lastResponse.jsonData["status"].toString() == "ok"));

        if (lastResponse.success && lastResponse.message == "move_saved") {
             emit PvPMoveSuccess(); /// 发出落子成功信号


        } else if(lastResponse.success && lastResponse.message == "waiting"){
            QTimer::singleShot(1000, this, [this]() {
                MovePvPPolling();
            });
        }else if(lastResponse.success && lastResponse.message == "opponent_moved"){
            int x = lastResponse.jsonData["x"].toInt();
            int y = lastResponse.jsonData["y"].toInt();
            qDebug() << "[HttpClient] Opponent move parsed: x =" << x << ", y =" << y;

            // 发出信号，通知 Connect2Server
            emit receivedOpponentMove(x, y);
        }else {
            qDebug()<<"回复代码错误";
        }

        reply->deleteLater();
    });
}

