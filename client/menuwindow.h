#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class MenuWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MenuWindow(QWidget *parent = nullptr);

signals:
    void aiGameSelected();
    void pvpGameSelected();
    void exitSelected();

private slots:
    void onAIGameClicked();
    void onPVPGameClicked();
    void onExitClicked();

private:
    QPushButton *aiGameButton_;
    QPushButton *pvpGameButton_;
    QPushButton *exitButton_;
};

#endif // MENUWINDOW_H
