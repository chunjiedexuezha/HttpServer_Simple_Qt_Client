#ifndef GOMOKUBOARD_H
#define GOMOKUBOARD_H

#include <QWidget>
#include <QVector>
#include <QTimer>
#include <QLabel>
class GomokuBoard : public QWidget
{
    Q_OBJECT
public:
    explicit GomokuBoard(QWidget *parent = nullptr);

    void undoMove();
    void restartGame();

    void setUserId(int id);   // 设置用户ID
    void reset();             // 重置棋盘
     void confirmMove(int row, int col); // 内部调用，确定落子

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    enum PieceType { EMPTY = 0, BLACK, WHITE };

    static constexpr int BOARD_SIZE = 15;
    static constexpr int GRID_SIZE = 30;        // 每格30px
    static constexpr int BOARD_MARGIN = 15;     // 边距15px
    static constexpr int PIECE_RADIUS = 13;     // 棋子半径
    int userId_ = -1;
    bool isMyTurn_;  // 是否轮到我方

    QPixmap yellowBearPixmap;  // 黄色熊头图片
    QPixmap brownBearPixmap;   // 棕色熊头图片


    PieceType board_[BOARD_SIZE][BOARD_SIZE];
    bool blackTurn_;
    bool selectionValid_;
    int selectedRow_;
    int selectedCol_;

    struct Move {
        int row;
        int col;
        PieceType piece;
    };
    QVector<Move> moves_;

    // 动画相关
    QTimer animationTimer_;
    float animationProgress_;
    int animRow_;
    int animCol_;
    PieceType animPiece_;

    void drawPiece(QPainter &painter, int row, int col, QPixmap* pieceImage, float opacity);
    void drawSelectionCircle(QPainter &painter, int row, int col);

    void startAnimation(int row, int col, PieceType piece);
private slots:
    void onAnimationTimeout();
     bool checkWin(int row, int col, int player);  // 检查某个方向是否五连

     // 收到对手落子，外部调用更新棋盘
     void opponentMove(int row, int col);

     // 新增槽函数
 public:
     void setMyTurn(bool myTurn);  // 外部设置是否轮到我方

signals:
    void gameStatusChanged(const QString &status);
     void gameOver(int winner);  // 1=黑，2=白
    // 玩家确定落子后，通知外部（Connect2Server）行列位置
    void playerMove(int row, int col);
    void turnChanged(bool isMyTurn);

};

#endif // GOMOKUBOARD_H
