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
#include "event.h"

class Window;
class QLabel;
class QLineEdit;
class QTimeEdit;
class QCheckBox;
class QPushButton;

class EditWindow : public QWidget{
Q_OBJECT
public:
    EditWindow(QString username, QString password, Event event, QNetworkAccessManager* manager, QWidget *parent = 0);

private slots:
    void enableEditRequestButton();

    void editEventRequest();
    void editEventRequestFinished();

signals:
    void editFinished();

private:
    QString username;
    QString password;

    QLabel *summary;
    QLabel *location;
    QLabel *start;
    QLabel *end;
    QLabel *day;

    Event event;

    QLineEdit *lineSummary;
    QLineEdit *lineLocation;
    QTimeEdit *lineStart;
    QTimeEdit *lineEnd;

    QPushButton *createButton;
    QPushButton *closeButton;

    QNetworkAccessManager* manager;
    QNetworkReply* putReply;
};

#endif // EDITWINDOW_H
