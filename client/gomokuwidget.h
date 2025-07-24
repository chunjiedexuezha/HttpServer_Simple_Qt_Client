#ifndef GOMOKUWIDGET_H
#define GOMOKUWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "gomokuboard.h"

class GomokuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GomokuWidget(QWidget *parent = nullptr);
     GomokuWidget(int gameId, bool isBlack, QWidget *parent = nullptr);

    void setUserId(int id);     // 设置用户 ID
    void resetBoard();          // 重置棋盘
    GomokuBoard* getBoard(){
        return m_board;
    }
    int getGameId(){
        return m_gameId;
    }

signals:
    void playerMoveConfirmed(int row, int col);
public slots:
    void onBoardMoveConfirmed(int row, int col);
    void updateTurnLabel(bool isMyTurn);
    void setOpponentMoved();
    // 用于接收网络通知，网络层调用这个槽设置己方是否回合
    void onSetMyTurn(bool myTurn);

private:
    GomokuBoard *m_board;
    QPushButton *undoButton_;
    QPushButton *restartButton_;
    int m_userId = -1;
    QLabel *statusLabel_; // 轮次
    // 新增成员变量
    int m_gameId = -1;
    bool m_isBlack = false;
    bool m_isMyTurn = false;
};

#endif // GOMOKUWIDGET_H
