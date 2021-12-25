#include "userdialog.h"

UserDialog::UserDialog( QWidget *parent): QDialog(parent){

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

void UserDialog::loginClicked(){
    QString username = lineEdit->text();
    QString password = passwordLineEdit->text();

    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinishedAuth(QNetworkReply*)));

    QUrl url("http://localhost/calendarserver.php/");
    url.setUserName(username);
    url.setPassword(password);
    QNetworkRequest request=QNetworkRequest(url);
    manager->get(request);
}

void UserDialog::replyFinishedAuth (QNetworkReply *reply){
    if(reply->error()){
        lineEdit->setText("");
        passwordLineEdit->setText("");
       QMessageBox::information(this, "Error", "AUTHENTICATION ERROR!");
        qDebug() << "AUTHENTICATION ERROR!";
        qDebug() << reply->errorString();
    }
    else{
        qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();
        qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

        QString username = lineEdit->text();
        QString password = passwordLineEdit->text();

        this->close();
        window = new ToDoApps(username,password);
        window->show();
    }
    reply->deleteLater();
}

void UserDialog::enableLoginButton(){
//Abilitazione del login
loginButton->setEnabled(!lineEdit->text().isEmpty() && !passwordLineEdit->text().isEmpty());
}
