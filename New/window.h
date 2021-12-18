#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QDateTime>
#include <QScrollArea>
#include <QListWidget>
#include <QWidgetAction>
#include <QCalendarWidget>

#include <QLabel>
#include <QAction>
#include <QLineEdit>
#include <QDateEdit>
#include <QToolButton>
#include <QPushButton>

#include <Qdir>
#include <QDebug>
#include <QBuffer>
#include <QLocale>

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QMessageBox>
#include <QGridLayout>

#include <QDomDocument>
#include <QtXml/QDomNode>
#include <QTextCharFormat>
#include <QtXml/QDomNodeList>

#include "todoapps.h"
#include "editwindow.h"
#include "networkmanager.h"

QT_BEGIN_NAMESPACE
class QDate;
class QLabel;
class QDateEdit;
class QLineEdit;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QGridLayout;
class QPushButton;
class QListWidget;
class QCalendarWidget;

QT_END_NAMESPACE

class EditWindow;

class Window : public QWidget{
Q_OBJECT

public:
    Window(QString username, QString password, QWidget *parent = nullptr);

    void getCalendar();
    void createCalendarObject();
    QString encodeBase64(QString string);
    void handleRequestSyncTokenFinished(void);

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
    //CALENDARIO
    void createDatesGroupBox();
    void createPreviewGroupBox();
    void createTextFormatsGroupBox();
    QComboBox *createColorComboBox();
    void createGeneralOptionsGroupBox();

    //TO-DO LIST
    void createSearchBar();
    void createLabelTasks();
    void createEditDeleteButtons();
    void addToView(QString summary, QString location, QString dtstart, QString dtend);

    //USER INFO
    QString username;
    QString password;

    //ADD EVENT WINDOW
    EditWindow* editWindow;

    //NETWORK MANAGER
    QNetworkAccessManager* manager;

    //EDIT E DELETE BUTTONS
    QPushButton* EditButton;
    QPushButton* DeleteButton;

    //SEARCHBAR
    QLineEdit* SearchBar;

    //TO-DO LIST
    ToDoApps* toDoList;
    QListWidget* ToDoListView;
    QListWidgetItem* ToDoListData;


    //CALENDARIO
    QGroupBox *previewGroupBox;

    QGridLayout *previewLayout;
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

    QLabel *weekdayColorLabel;
    QLabel *weekendColorLabel;
    QLabel *headerTextFormatLabel;
    QComboBox *weekdayColorCombo;
    QComboBox *weekendColorCombo;
    QComboBox *headerTextFormatCombo;

    QCheckBox *firstFridayCheckBox;
    QCheckBox *mayFirstCheckBox;
};

#endif // WINDOW_H
