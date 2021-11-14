//
// Created by Giovanni on 05/11/2021.
//

#ifndef TODOLIST_CALENDAR_FINDDIALOG_H
#define TODOLIST_CALENDAR_FINDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

class FindDialog : public QDialog {
Q_OBJECT

public:
     FindDialog(QWidget *parent = 0);
signals:
     void findNext(const QString &str, Qt::CaseSensitivity cs);
     void findPrevious(const QString &str, Qt::CaseSensitivity cs);

     private slots:
         void findClicked();
         void enableFindButton(const QString &text);

     private:
        QLabel *label;
         QLineEdit *lineEdit;
        QCheckBox *caseCheckBox;
        QCheckBox *backwardCheckBox;
        QPushButton *findButton;
        QPushButton *closeButton;
};

#endif //TODOLIST_CALENDAR_FINDDIALOG_H
