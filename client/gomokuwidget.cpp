#include "gomokuwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

GomokuWidget::GomokuWidget(QWidget *parent)
    : QWidget(parent)
{
    m_board = new GomokuBoard(this);
    undoButton_ = new QPushButton(tr("撤销"), this);
    restartButton_ = new QPushButton(tr("重新开始"), this);
    statusLabel_ = new QLabel("轮到我方", this); // 默认我方先手

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(undoButton_);
    buttonLayout->addWidget(restartButton_);
    buttonLayout->addWidget(statusLabel_);
    buttonLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_board);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();

    connect(m_board, &GomokuBoard::playerMove,
            this, &GomokuWidget::onBoardMoveConfirmed);
    connect(undoButton_, &QPushButton::clicked, m_board, &GomokuBoard::undoMove);
    connect(restartButton_, &QPushButton::clicked, m_board, &GomokuBoard::restartGame);
    connect(m_board, &GomokuBoard::gameOver, this, [=](int winner) {
        QString msg = (winner == 1 ? "黑棋" : "白棋") + QString("胜利！");
        QMessageBox::information(this, "游戏结束", msg);
    });
    connect(m_board, &GomokuBoard::turnChanged, this, &GomokuWidget::updateTurnLabel);

    m_isMyTurn = true;
    updateTurnLabel(m_isMyTurn);
}

GomokuWidget::GomokuWidget(int gameId, bool isBlack, QWidget *parent)
    : QWidget(parent), m_gameId(gameId), m_isBlack(isBlack)
{
    setWindowTitle("五子棋对局窗口");


    m_board = new GomokuBoard(this);
    m_board->setMyTurn(isBlack);
    undoButton_ = new QPushButton(tr("撤销"), this);
    restartButton_ = new QPushButton(tr("重新开始"), this);
    statusLabel_ = new QLabel(this);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(undoButton_);
    buttonLayout->addWidget(restartButton_);
    buttonLayout->addWidget(statusLabel_);
    buttonLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_board);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();

    connect(m_board, &GomokuBoard::playerMove,
            this, &GomokuWidget::onBoardMoveConfirmed);
    connect(undoButton_, &QPushButton::clicked, m_board, &GomokuBoard::undoMove);
    connect(restartButton_, &QPushButton::clicked, m_board, &GomokuBoard::restartGame);
    connect(m_board, &GomokuBoard::gameOver, this, [=](int winner) {
        QString msg = (winner == 1 ? "黑棋" : "白棋") + QString("胜利！");
        QMessageBox::information(this, "游戏结束", msg);
    });
    connect(m_board, &GomokuBoard::turnChanged, this, &GomokuWidget::updateTurnLabel);

    m_isMyTurn = true; // 黑先手，白后手
    updateTurnLabel(m_isBlack);
    m_board->reset();

}

void GomokuWidget::setUserId(int id)
{
    m_userId = id;
    m_board->setUserId(id);
}

void GomokuWidget::resetBoard()
{
    m_board->reset();
    m_isMyTurn = m_isBlack;
    updateTurnLabel(m_isMyTurn);
}

void GomokuWidget::onBoardMoveConfirmed(int row, int col)
{
    qDebug()<<"[onBoardMoveConfirmed]收到信号";
    qDebug()<<m_isMyTurn;
    if (!m_isMyTurn) return; // 非自己回合忽略

    emit playerMoveConfirmed(row, col);
    qDebug()<<"playerMoveConfirmed信号发出";
    m_isMyTurn = false;
    updateTurnLabel(m_isMyTurn);
}

void GomokuWidget::updateTurnLabel(bool isMyTurn)
{
    if (isMyTurn) {
        statusLabel_->setText("轮到我方");
        statusLabel_->setStyleSheet("QLabel { font-weight: bold; font-size: 16px; color: green; }");
    } else {
        statusLabel_->setText("轮到对方");
        statusLabel_->setStyleSheet("QLabel { font-weight: bold; font-size: 16px; color: red; }");
    }
}

void GomokuWidget::setOpponentMoved()
{
    m_isMyTurn = true;
    updateTurnLabel(m_isMyTurn);
}

void GomokuWidget::onSetMyTurn(bool myTurn)
{
    m_isMyTurn = myTurn;
    updateTurnLabel(myTurn);
}
