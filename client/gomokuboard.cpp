#include "gomokuboard.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

GomokuBoard::GomokuBoard(QWidget *parent)
    : QWidget(parent),
    blackTurn_(true),
    selectionValid_(false),
    selectedRow_(-1),
    selectedCol_(-1),
    isMyTurn_(false),  // 默认不轮到自己
    animationProgress_(0.0f),
    animRow_(-1),
    animCol_(-1),

    animPiece_(EMPTY)



{
    setFixedSize(BOARD_MARGIN * 2 + GRID_SIZE * (BOARD_SIZE - 1),
                 BOARD_MARGIN * 2 + GRID_SIZE * (BOARD_SIZE - 1));

    // 初始化棋盘为空
    for (int r = 0; r < BOARD_SIZE; ++r)
        for (int c = 0; c < BOARD_SIZE; ++c)
            board_[r][c] = EMPTY;

    connect(&animationTimer_, &QTimer::timeout, this, &GomokuBoard::onAnimationTimeout);

    animationTimer_.setInterval(20);
    yellowBearPixmap.load(":/黄色熊头.png");
    brownBearPixmap.load(":/棕色熊头.png");

    // 检查图片是否加载成功，失败时使用备用颜色
    if(yellowBearPixmap.isNull()) qWarning() << "Failed to load yellow bear image";
    if(brownBearPixmap.isNull()) qWarning() << "Failed to load brown bear image";
}

void GomokuBoard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 经典淡木色背景
    QColor woodColor(242, 197, 143);
    painter.fillRect(rect(), woodColor);

    QPen pen(QColor(102, 51, 0)); // 深棕色网格线
    pen.setWidth(1);
    painter.setPen(pen);

    // 画横竖线
    for (int i = 0; i < BOARD_SIZE; ++i) {
        int pos = BOARD_MARGIN + i * GRID_SIZE;
        painter.drawLine(BOARD_MARGIN, pos, BOARD_MARGIN + (BOARD_SIZE - 1) * GRID_SIZE, pos);
        painter.drawLine(pos, BOARD_MARGIN, pos, BOARD_MARGIN + (BOARD_SIZE - 1) * GRID_SIZE);
    }

    // 画已有棋子
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (board_[r][c] != EMPTY) {
                float opacity = 1.0f;
                // 动画棋子不绘制这里
                if (r == animRow_ && c == animCol_)
                    continue;

                // 修改点1：使用图片代替颜色绘制
                QPixmap* pieceImage = (board_[r][c] == BLACK) ? &brownBearPixmap : &yellowBearPixmap;
                drawPiece(painter, r, c, pieceImage, opacity);
            }
        }
    }

    // 动画棋子绘制
    if (animRow_ >= 0 && animCol_ >= 0 && animPiece_ != EMPTY) {
        // 修改点2：动画棋子同样使用图片
        QPixmap* pieceImage = (animPiece_ == BLACK) ? &brownBearPixmap : &yellowBearPixmap;
        drawPiece(painter, animRow_, animCol_, pieceImage, animationProgress_);
    }

    // 画选中虚线红圈
    if (selectionValid_) {
        drawSelectionCircle(painter, selectedRow_, selectedCol_);
    }
}

// 修改drawPiece函数以支持图片绘制
void GomokuBoard::drawPiece(QPainter &painter, int row, int col, QPixmap* pieceImage, float opacity)
{
    int centerX = BOARD_MARGIN + col * GRID_SIZE;
    int centerY = BOARD_MARGIN + row * GRID_SIZE;
    int pieceRadius = PIECE_RADIUS;  // 棋子半径，例如GRID_SIZE/2 - 2

    // 保存原始状态
    painter.save();

    // 设置透明度
    painter.setOpacity(opacity);

    // 如果有有效的图片，则绘制图片
    if (pieceImage && !pieceImage->isNull()) {
        // 计算绘制区域
        QRect targetRect(centerX - pieceRadius, centerY - pieceRadius,
                         pieceRadius * 2, pieceRadius * 2);

        // 平滑缩放图片
        painter.drawPixmap(targetRect,
                           pieceImage->scaled(targetRect.size(),
                                              Qt::KeepAspectRatio,
                                              Qt::SmoothTransformation));
    }
    else {
        // 备用方案：图片加载失败时使用彩色圆形
        QColor fallbackColor = (pieceImage == &brownBearPixmap) ?
                                   QColor(101, 67, 33) : // 棕色
                                   QColor(255, 204, 0);   // 黄色

        painter.setBrush(fallbackColor);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(centerX - pieceRadius, centerY - pieceRadius,
                            pieceRadius * 2, pieceRadius * 2);
    }

    // 恢复原始状态
    painter.restore();
}

void GomokuBoard::drawSelectionCircle(QPainter &painter, int row, int col)
{
    QPoint center(BOARD_MARGIN + col * GRID_SIZE, BOARD_MARGIN + row * GRID_SIZE);
    QPen pen(Qt::red);
    pen.setStyle(Qt::DashLine);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(center, PIECE_RADIUS, PIECE_RADIUS);
}

void GomokuBoard::mousePressEvent(QMouseEvent *event)
{
    if (!isMyTurn_) {
        qDebug() << "Not your turn!";
        return;  // 非我方回合不能下棋
    }
    int x = event->pos().x();
    int y = event->pos().y();

    int col = (x - BOARD_MARGIN + GRID_SIZE / 2) / GRID_SIZE;
    int row = (y - BOARD_MARGIN + GRID_SIZE / 2) / GRID_SIZE;

    if (col < 0 || col >= BOARD_SIZE || row < 0 || row >= BOARD_SIZE) {
        return;
    }

    if (board_[row][col] != EMPTY) {
        // 该位置已有棋子，不允许选中
        return;
    }

    if (!selectionValid_) {
        // 第一次点击：显示红色虚线圈
        selectedRow_ = row;
        selectedCol_ = col;
        selectionValid_ = true;
        update();
    } else {
        // 第二次点击：如果位置一样则确认下棋，否则切换选中
        if (selectedRow_ == row && selectedCol_ == col) {
            // 落子
            confirmMove(row, col);
            qDebug() << "下棋";
        } else {
            // 改变选中位置
            selectedRow_ = row;
            selectedCol_ = col;
        }
        update();
    }
}

void GomokuBoard::startAnimation(int row, int col, PieceType piece)
{
    animRow_ = row;
    animCol_ = col;
    animPiece_ = piece;
    animationProgress_ = 0.0f;
    animationTimer_.start();
}

void GomokuBoard::onAnimationTimeout()
{
    animationProgress_ += 0.1f;
    if (animationProgress_ >= 1.0f) {
        animationProgress_ = 1.0f;
        animationTimer_.stop();

        // 动画结束，重置动画棋子参数
        animRow_ = -1;
        animCol_ = -1;
        animPiece_ = EMPTY;
    }
    update();
}

void GomokuBoard::undoMove()
{
    if (moves_.isEmpty()) return;

    Move last = moves_.takeLast();
    board_[last.row][last.col] = EMPTY;

    // 如果动画棋子是空，撤销后棋手要回退
    blackTurn_ = (last.piece == BLACK);

    // 清除选中
    selectionValid_ = false;
    selectedRow_ = -1;
    selectedCol_ = -1;

    update();
}

void GomokuBoard::restartGame()
{
    for (int r = 0; r < BOARD_SIZE; ++r)
        for (int c = 0; c < BOARD_SIZE; ++c)
            board_[r][c] = EMPTY;

    moves_.clear();
    blackTurn_ = true;
    selectionValid_ = false;
    selectedRow_ = -1;
    selectedCol_ = -1;

    animRow_ = -1;
    animCol_ = -1;
    animPiece_ = EMPTY;
    animationTimer_.stop();
    animationProgress_ = 0.0f;

    update();
}

bool GomokuBoard::checkWin(int row, int col, int player)
{
    // 定义四个方向：横、竖、主对角、副对角
    const int directions[4][2] = {
        {0, 1},   // 横向
        {1, 0},   // 竖向
        {1, 1},   // 左上→右下
        {1, -1}   // 右上→左下
    };

    for (auto &dir : directions) {
        int count = 1;

        // 向正方向统计
        int r = row + dir[0], c = col + dir[1];
        while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && board_[r][c] == player) {
            count++;
            r += dir[0];
            c += dir[1];
        }

        // 向反方向统计
        r = row - dir[0], c = col - dir[1];
        while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && board_[r][c] == player) {
            count++;
            r -= dir[0];
            c -= dir[1];
        }

        if (count >= 5)
            return true;
    }

    return false;
}

void GomokuBoard::setUserId(int id)
{
    userId_ = id;
}

void GomokuBoard::reset()
{
    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        for (int col = 0; col < BOARD_SIZE; ++col)
        {
            board_[row][col] = EMPTY;
        }
    }
    moves_.clear();
    selectionValid_ = false;
    selectedRow_ = -1;
    selectedCol_ = -1;
    blackTurn_ = true;

    update();
}

void GomokuBoard::confirmMove(int row, int col)
{
    // 先做本地落子动画等逻辑
    PieceType piece = blackTurn_ ? BLACK : WHITE;

    startAnimation(row, col, piece);
    moves_.append({row, col, piece});
    board_[row][col] = piece;

    // 胜负检测
    if (checkWin(row, col, piece)) {
        emit gameOver(piece);  // 触发胜负信号
        restartGame();
    }
    blackTurn_ = !blackTurn_;

    // 发出信号通知外部
    emit playerMove(row, col);
    qDebug() << "[confirmMove]:下棋确认并发送信号playerMove";
    // 清除选择
    selectionValid_ = false;
    selectedRow_ = -1;
    selectedCol_ = -1;

}

void GomokuBoard::opponentMove(int row, int col)
{
    // 对方落子，黑白棋转换根据协议定（假设对方总是另一颜色）
    PieceType piece = blackTurn_ ? BLACK : WHITE;

    startAnimation(row, col, piece);
    moves_.append({row, col, piece});
    board_[row][col] = piece;
    blackTurn_ = !blackTurn_;

    update();
}

// GomokuBoard.cpp

void GomokuBoard::setMyTurn(bool myTurn)
{
    isMyTurn_ = myTurn;
    emit turnChanged(isMyTurn_);
    update();

}
