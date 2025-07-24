#ifndef LOGINREGISTERWIDGET_H
#define LOGINREGISTERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

class LoginRegisterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginRegisterWidget(QWidget *parent = nullptr);

    QString getUsername() const;
    QString getPassword() const;

signals:
    void loginClicked();
    void registerClicked();

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;

};

#endif // LOGINREGISTERWIDGET_H
