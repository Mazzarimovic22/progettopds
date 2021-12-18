#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QtGui>
#include <QDebug>
#include <QLabel>
#include <QDialog>
#include <QTimeEdit>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>

#include "networkmanager.h"

class Window;
class QLabel;
class QLineEdit;
class QTimeEdit;
class QCheckBox;
class QPushButton;

class EditWindow : public QWidget{
Q_OBJECT
public:
    EditWindow(QString username, QString password, QDate date, QNetworkAccessManager* manager, QWidget *parent = 0);

private slots:
    void editRequest();
    void enableEditRequestButton();

private:
    QString username;
    QString password;

    QLabel *summary;
    QLabel *location;
    QLabel *start;
    QLabel *end;
    QLabel *day;

    QDate date;

    QLineEdit *lineSummary;
    QLineEdit *lineLocation;
    QTimeEdit *lineStart;
    QTimeEdit *lineEnd;

    QPushButton *createButton;
    QPushButton *closeButton;

    QNetworkAccessManager* manager;
};

#endif // EDITWINDOW_H
