#include "menuwindow.h"

MenuWindow::MenuWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("菜单");
    setFixedSize(300, 200);

    aiGameButton_ = new QPushButton("人机对战");
    pvpGameButton_ = new QPushButton("玩家对战");
    exitButton_ = new QPushButton("退出游戏");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(aiGameButton_);
    layout->addWidget(pvpGameButton_);
    layout->addWidget(exitButton_);

    setLayout(layout);

    connect(aiGameButton_, &QPushButton::clicked, this, &MenuWindow::onAIGameClicked);
    connect(pvpGameButton_, &QPushButton::clicked, this, &MenuWindow::onPVPGameClicked);
    connect(exitButton_, &QPushButton::clicked, this, &MenuWindow::onExitClicked);
}

void MenuWindow::onAIGameClicked()
{
    emit aiGameSelected();
}

void MenuWindow::onPVPGameClicked()
{
    emit pvpGameSelected();
}

void MenuWindow::onExitClicked()
{
    emit exitSelected();
}
