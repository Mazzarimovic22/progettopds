//
// Created by Giovanni on 05/11/2021.
//

#include "UserDialog.h"

UserDialog::UserDialog(QWidget *parent): QDialog(parent) {

    //Username e Password
    label = new QLabel(tr("Username:"));
    passwordLabel = new QLabel(tr("Password:"));
    lineEdit = new QLineEdit;
    passwordLineEdit = new QLineEdit;
    label->setBuddy(lineEdit);
    passwordLabel->setBuddy(passwordLineEdit);

    //Login button
    loginButton = new QPushButton(tr("&Login"));
    loginButton->setFixedWidth(70);
    //loginButton->setStyleSheet("font: bold;background-color: red;font-size: 36px;height: 48px;width: 2px;");
    loginButton->setDefault(true);
    loginButton->setEnabled(false);

    //Close button
    closeButton = new QPushButton(tr("Close"));
    closeButton->setFixedWidth(70);

    connect(lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableLoginButton()));
    connect(passwordLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableLoginButton()));
    connect(loginButton, SIGNAL(clicked()), this, SLOT(loginClicked()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));


    QVBoxLayout *userTextLayout = new QVBoxLayout;
    userTextLayout->addWidget(label);
    userTextLayout->addWidget(passwordLabel);

    QVBoxLayout *insertTextLayout = new QVBoxLayout;
    insertTextLayout->addWidget(lineEdit);
    insertTextLayout->addWidget(passwordLineEdit);

    QHBoxLayout *smallBoxLayout = new QHBoxLayout;
    smallBoxLayout->addLayout(userTextLayout);
    smallBoxLayout->addLayout(insertTextLayout);

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->setAlignment(Qt::AlignHCenter);
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(closeButton);

    QVBoxLayout *bigBoxLayout = new QVBoxLayout;

    bigBoxLayout->addLayout(smallBoxLayout);
    bigBoxLayout->addLayout(buttonLayout);
    setLayout(bigBoxLayout);

    setWindowTitle(tr("Login"));
    setFixedHeight(sizeHint().height());
}

void UserDialog::loginClicked() {
    QString textUsername = lineEdit->text();
    QString textPassword = passwordLineEdit->text();


    //SERVER
    if(textUsername == "admin" && textPassword == "admin") {
        window = new Window();
        window->show();
        this->close();
    }
    else {
        lineEdit->setText("");
        passwordLineEdit->setText("");
    }
}

void UserDialog::enableLoginButton()
{
    //Abilitazione del login
    loginButton->setEnabled(!lineEdit->text().isEmpty() && !passwordLineEdit->text().isEmpty());
}

