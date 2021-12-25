#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QDateTime>
#include <QScrollArea>
#include <QListWidget>
#include <QAction>
#include <QToolButton>
#include <QWidgetAction>
#include <QPushButton>
#include <QLineEdit>
#include <Qdir>

#include <QDebug>
#include "networkmanager.h"
#include <QMessageBox>
#include <QBuffer>
#include "editwindow.h"

#include <QCalendarWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLocale>
#include <QTextCharFormat>
#include <QtXml/QDomNode>
#include <QtXml/QDomNodeList>
#include <QDomDocument>

QT_BEGIN_NAMESPACE
class QCalendarWidget;
class QCheckBox;
class QComboBox;
class QDate;
class QDateEdit;
class QGridLayout;
class QGroupBox;
class QLabel;
class QListWidget;
class QPushButton;
class QLineEdit;
QT_END_NAMESPACE

class MainWindow : public QWidget {
Q_OBJECT
public:
    MainWindow(QString username, QString password, QWidget *parent = nullptr);


private slots:
    void localeChanged(int index);
    void firstDayChanged(int index);
    void selectionModeChanged(int index);
    void horizontalHeaderChanged(int index);
    void verticalHeaderChanged(int index);
    void selectedDateChanged();
    void minimumDateChanged(QDate date);
    void maximumDateChanged(QDate date);
    void weekdayFormatChanged();
    void weekendFormatChanged();
    void reformatHeaders();
    void reformatCalendarPage();

    void editClicked(); //Click EditButton
    void deleteClicked(); //Click DeleteButton

private:

    void createEditDeleteButtons(); //NEW PDS POLPETTA
    void createSearchBar();
    void createPreviewGroupBox();






    QString username;
    QString password;

    QPushButton* EditButton; //POLPETTA
    QPushButton* DeleteButton; //POLPETTA

    QLineEdit* SearchBar; //POLPETTA

    QGroupBox *previewGroupBox;
    QGroupBox *previewGroupBox2; //PDS

    QGridLayout *previewLayout;
    QGridLayout *previewLayout2; //PDS
    QCalendarWidget *calendar;

    QGroupBox *generalOptionsGroupBox;
    QLabel *localeLabel;
    QLabel *firstDayLabel;

    QLabel *selectionModeLabel;
    QLabel *horizontalHeaderLabel;
    QLabel *verticalHeaderLabel;
    QComboBox *localeCombo;
    QComboBox *firstDayCombo;
    QComboBox *selectionModeCombo;
    QCheckBox *gridCheckBox;
    QCheckBox *navigationCheckBox;
    QComboBox *horizontalHeaderCombo;
    QComboBox *verticalHeaderCombo;

    QGroupBox *datesGroupBox;
    QLabel *currentDateLabel;
    QLabel *minimumDateLabel;
    QLabel *maximumDateLabel;
    QDateEdit *currentDateEdit;
    QDateEdit *minimumDateEdit;
    QDateEdit *maximumDateEdit;

    QGroupBox *textFormatsGroupBox;

    QVBoxLayout* eventList;
    QLabel *selectedDay;

    QLabel *weekdayColorLabel;
    QLabel *weekendColorLabel;
    QLabel *headerTextFormatLabel;
    QComboBox *weekdayColorCombo;
    QComboBox *weekendColorCombo;
    QComboBox *headerTextFormatCombo;

    QCheckBox *firstFridayCheckBox;
    QCheckBox *mayFirstCheckBox;


};

#endif // MAINWINDOW_H
