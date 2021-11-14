//
// Created by Giovanni on 18/10/2021.
//

#ifndef TODOLIST_CALENDAR_WINDOW_H
#define TODOLIST_CALENDAR_WINDOW_H

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

//! [0]
class Window : public QWidget
{
Q_OBJECT

public:
    Window(QWidget *parent = nullptr);

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

private:
    void createPreviewGroupBox();
    void createGeneralOptionsGroupBox();
    void createDatesGroupBox();
    void createTextFormatsGroupBox();
    QComboBox *createColorComboBox();

    //CODICE PDS
    void createLabelTasks();

    void createEditDeleteButtons(); //NEW PDS POLPETTA
    void createSearchBar();

    QGroupBox *previewGroupBox;
    QGroupBox *previewGroupBox2; //PDS

    QGridLayout *previewLayout;
    QGridLayout *previewLayout2; //PDS
    QCalendarWidget *calendar;

    QGroupBox *generalOptionsGroupBox;
    QLabel *localeLabel;
    QLabel *firstDayLabel;
//! [0]
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

    QListWidget* ToDoListView; //PDS

    QListWidgetItem* ToDoListData; //PDS


    QPushButton* EditButton; //POLPETTA
    QPushButton* DeleteButton; //POLPETTA

    QLineEdit* SearchBar; //POLPETTA

    QLabel *weekdayColorLabel;
    QLabel *weekendColorLabel;
    QLabel *headerTextFormatLabel;
    QComboBox *weekdayColorCombo;
    QComboBox *weekendColorCombo;
    QComboBox *headerTextFormatCombo;

    QCheckBox *firstFridayCheckBox;
//! [1]
    QCheckBox *mayFirstCheckBox;


};
//! [1]

#endif //TODOLIST_CALENDAR_WINDOW_H
