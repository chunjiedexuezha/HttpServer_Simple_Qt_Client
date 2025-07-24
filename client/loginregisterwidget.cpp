#include "loginregisterwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

LoginRegisterWidget::LoginRegisterWidget(QWidget *parent)
    : QWidget(parent)
{
    QLabel *userLabel = new QLabel("账号:", this);
    QLabel *passLabel = new QLabel("密码:", this);

    usernameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);

    // 横向布局：账号
    QHBoxLayout *userLayout = new QHBoxLayout;
    userLayout->addWidget(userLabel);
    userLayout->addWidget(usernameEdit);

    // 横向布局：密码
    QHBoxLayout *passLayout = new QHBoxLayout;
    passLayout->addWidget(passLabel);
    passLayout->addWidget(passwordEdit);

    loginButton = new QPushButton("登录", this);
    registerButton = new QPushButton("注册", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(userLayout);
    mainLayout->addLayout(passLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    connect(loginButton, &QPushButton::clicked, this, &LoginRegisterWidget::loginClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginRegisterWidget::registerClicked);
}

QString LoginRegisterWidget::getUsername() const
{
    return usernameEdit->text();
}

QString LoginRegisterWidget::getPassword() const
{
    return passwordEdit->text();
}
