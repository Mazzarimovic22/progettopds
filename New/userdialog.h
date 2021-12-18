#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QtGui>
#include <QDebug>
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>

#include "window.h"
#include "networkmanager.h"

class Window;
class QLabel;
class QLineEdit;
class QCheckBox;
class QPushButton;

class UserDialog : public QDialog{
Q_OBJECT

public:
    UserDialog(QWidget *parent = 0);

private slots:
    void loginClicked();
    void enableLoginButton();
    void replyFinishedAuth (QNetworkReply *reply);

private:
    QLabel *label;
    QLabel *passwordLabel;

    QLineEdit *lineEdit;
    QLineEdit *passwordLineEdit;

    QCheckBox *caseCheckBox;
    QCheckBox *backwardCheckBox;

    QPushButton *loginButton;
    QPushButton *closeButton;

    Window* window;
    QNetworkAccessManager* manager;
};

#endif // USERDIALOG_H
