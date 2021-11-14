//
// Created by Giovanni on 05/11/2021.
//

#ifndef TODOLIST_CALENDAR_USERDIALOG_H
#define TODOLIST_CALENDAR_USERDIALOG_H


#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QtGui>
#include <iostream>
#include "Window.h"

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class Window;

class UserDialog : public QDialog {
Q_OBJECT

public:
    UserDialog(QWidget *parent = 0);
signals:
    void findNext(const QString &str, Qt::CaseSensitivity cs);
    void findPrevious(const QString &str, Qt::CaseSensitivity cs);

private slots:
    void loginClicked();
    void enableLoginButton();

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
};

#endif //TODOLIST_CALENDAR_USERDIALOG_H
