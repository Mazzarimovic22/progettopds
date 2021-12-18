//
// Created by Giovanni on 18/10/2021.
//

#include "Window.h"
#include <iostream>

#include <QCalendarWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLocale>
#include <QTextCharFormat>
#include "Data.h"

//! [0]
Window::Window(QWidget *parent)
        : QWidget(parent)
{
    createPreviewGroupBox();
    createGeneralOptionsGroupBox();
    createDatesGroupBox();
    createTextFormatsGroupBox();

    //CODICE PDS
    createLabelTasks();

    createEditDeleteButtons(); //POLPETTA
    createSearchBar();

    QGridLayout *layout = new QGridLayout;

    //CODICE PDS
    layout->addWidget(SearchBar,0,0);
    layout->addWidget(ToDoListView,1,0);
    layout->addWidget(calendar, 1, 1);

    layout->addWidget(EditButton,2,0); //POLPETTA
    layout->addWidget(DeleteButton,2,1);


    /*layout->addWidget(label_tasks,0,2);
    layout->addWidget(ToDoListGroupBox,0,0);
    //layout->addWidget(previewGroupBox, 1, 1);
    layout->addWidget(generalOptionsGroupBox, 0, 1);
    layout->addWidget(datesGroupBox, 1, 0);
    layout->addWidget(textFormatsGroupBox, 0, 2);*/
    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);

    previewLayout->setRowMinimumHeight(0, calendar->sizeHint().height());
    previewLayout->setColumnMinimumWidth(0, calendar->sizeHint().width());

    setWindowTitle(tr("Calendario e To-Do List"));
}
//! [0]

void Window::localeChanged(int index)
{
    const QLocale newLocale(localeCombo->itemData(index).toLocale());
    calendar->setLocale(newLocale);
    int newLocaleFirstDayIndex = firstDayCombo->findData(newLocale.firstDayOfWeek());
    firstDayCombo->setCurrentIndex(newLocaleFirstDayIndex);
}

//! [1]
void Window::firstDayChanged(int index)
{
    calendar->setFirstDayOfWeek(Qt::DayOfWeek(
            firstDayCombo->itemData(index).toInt()));
}
//! [1]

void Window::selectionModeChanged(int index)
{
    calendar->setSelectionMode(QCalendarWidget::SelectionMode(
            selectionModeCombo->itemData(index).toInt()));
}

void Window::horizontalHeaderChanged(int index)
{
    calendar->setHorizontalHeaderFormat(QCalendarWidget::HorizontalHeaderFormat(
            horizontalHeaderCombo->itemData(index).toInt()));
}

void Window::verticalHeaderChanged(int index)
{
    calendar->setVerticalHeaderFormat(QCalendarWidget::VerticalHeaderFormat(
            verticalHeaderCombo->itemData(index).toInt()));
}

//! [2]
void Window::selectedDateChanged()
{
    currentDateEdit->setDate(calendar->selectedDate());
    std::string currentDate = std::to_string(calendar->selectedDate().day()) + "/"
                              + std::to_string(calendar->selectedDate().month()) + "/" + std::to_string(calendar->selectedDate().year());

    ToDoListData->setText(currentDate.c_str());
}
//! [2]

//! [3]
void Window::minimumDateChanged(QDate date)
{
    calendar->setMinimumDate(date);
    maximumDateEdit->setDate(calendar->maximumDate());
}
//! [3]

//! [4]
void Window::maximumDateChanged(QDate date)
{
    calendar->setMaximumDate(date);
    minimumDateEdit->setDate(calendar->minimumDate());
}
//! [4]

//! [5]
void Window::weekdayFormatChanged()
{
    QTextCharFormat format;

    format.setForeground(qvariant_cast<QColor>(
            weekdayColorCombo->itemData(weekdayColorCombo->currentIndex())));
    calendar->setWeekdayTextFormat(Qt::Monday, format);
    calendar->setWeekdayTextFormat(Qt::Tuesday, format);
    calendar->setWeekdayTextFormat(Qt::Wednesday, format);
    calendar->setWeekdayTextFormat(Qt::Thursday, format);
    calendar->setWeekdayTextFormat(Qt::Friday, format);
}
//! [5]

//! [6]
void Window::weekendFormatChanged()
{
    QTextCharFormat format;

    format.setForeground(qvariant_cast<QColor>(
            weekendColorCombo->itemData(weekendColorCombo->currentIndex())));
    calendar->setWeekdayTextFormat(Qt::Saturday, format);
    calendar->setWeekdayTextFormat(Qt::Sunday, format);
}
//! [6]

//! [7]
void Window::reformatHeaders()
{
    QString text = headerTextFormatCombo->currentText();
    QTextCharFormat format;

    if (text == tr("Bold"))
        format.setFontWeight(QFont::Bold);
    else if (text == tr("Italic"))
        format.setFontItalic(true);
    else if (text == tr("Green"))
        format.setForeground(Qt::green);
    calendar->setHeaderTextFormat(format);
}
//! [7]

//! [8]
void Window::reformatCalendarPage()
{
    QTextCharFormat mayFirstFormat;
    const QDate mayFirst(calendar->yearShown(), 5, 1);

    QTextCharFormat firstFridayFormat;
    QDate firstFriday(calendar->yearShown(), calendar->monthShown(), 1);
    while (firstFriday.dayOfWeek() != Qt::Friday)
        firstFriday = firstFriday.addDays(1);

    if (firstFridayCheckBox->isChecked()) {
        firstFridayFormat.setForeground(Qt::blue);
    } else { // Revert to regular colour for this day of the week.
        Qt::DayOfWeek dayOfWeek(static_cast<Qt::DayOfWeek>(firstFriday.dayOfWeek()));
        firstFridayFormat.setForeground(calendar->weekdayTextFormat(dayOfWeek).foreground());
    }

    calendar->setDateTextFormat(firstFriday, firstFridayFormat);

    // When it is checked, "May First in Red" always takes precedence over "First Friday in Blue".
    if (mayFirstCheckBox->isChecked()) {
        mayFirstFormat.setForeground(Qt::red);
    } else if (!firstFridayCheckBox->isChecked() || firstFriday != mayFirst) {
        // We can now be certain we won't be resetting "May First in Red" when we restore
        // may 1st's regular colour for this day of the week.
        Qt::DayOfWeek dayOfWeek(static_cast<Qt::DayOfWeek>(mayFirst.dayOfWeek()));
        calendar->setDateTextFormat(mayFirst, calendar->weekdayTextFormat(dayOfWeek));
    }

    calendar->setDateTextFormat(mayFirst, mayFirstFormat);
}
//! [8]

//! [9]
void Window::createPreviewGroupBox()
{
    /*previewGroupBox = new QGroupBox(tr("Calendario"));
    previewGroupBox->setMinimumWidth(300); //PDS
    previewGroupBox->setMinimumHeight(280); //PDS*/
    calendar = new QCalendarWidget;
    calendar->setMinimumHeight(280);
    calendar->setMinimumHeight(300);

    calendar->setMinimumDate(QDate(1900, 1, 1));
    calendar->setMaximumDate(QDate(3000, 1, 1));
    calendar->setGridVisible(true);

    connect(calendar, &QCalendarWidget::currentPageChanged,
            this, &Window::reformatCalendarPage);

    previewLayout = new QGridLayout;
    previewLayout->addWidget(calendar, 0, 0, Qt::AlignCenter);

    /*previewGroupBox->setLayout(previewLayout);
    */
}


//! [9]

//! [10]
void Window::createGeneralOptionsGroupBox()
{
    generalOptionsGroupBox = new QGroupBox(tr("General Options"));

    localeCombo = new QComboBox;
    int curLocaleIndex = -1;
    int index = 0;
    for (int _lang = QLocale::C; _lang <= QLocale::LastLanguage; ++_lang) {
        QLocale::Language lang = static_cast<QLocale::Language>(_lang);
        QList<QLocale::Country> countries = QLocale::countriesForLanguage(lang);
        for (int i = 0; i < countries.count(); ++i) {
            QLocale::Country country = countries.at(i);
            QString label = QLocale::languageToString(lang);
            label += QLatin1Char('/');
            label += QLocale::countryToString(country);
            QLocale locale(lang, country);
            if (this->locale().language() == lang && this->locale().country() == country)
                curLocaleIndex = index;
            localeCombo->addItem(label, locale);
            ++index;
        }
    }
    if (curLocaleIndex != -1)
        localeCombo->setCurrentIndex(curLocaleIndex);
    localeLabel = new QLabel(tr("&Locale"));
    localeLabel->setBuddy(localeCombo);

    firstDayCombo = new QComboBox;
    firstDayCombo->addItem(tr("Sunday"), Qt::Sunday);
    firstDayCombo->addItem(tr("Monday"), Qt::Monday);
    firstDayCombo->addItem(tr("Tuesday"), Qt::Tuesday);
    firstDayCombo->addItem(tr("Wednesday"), Qt::Wednesday);
    firstDayCombo->addItem(tr("Thursday"), Qt::Thursday);
    firstDayCombo->addItem(tr("Friday"), Qt::Friday);
    firstDayCombo->addItem(tr("Saturday"), Qt::Saturday);

    firstDayLabel = new QLabel(tr("Wee&k starts on:"));
    firstDayLabel->setBuddy(firstDayCombo);
//! [10]

    selectionModeCombo = new QComboBox;
    selectionModeCombo->addItem(tr("Single selection"),
                                QCalendarWidget::SingleSelection);
    selectionModeCombo->addItem(tr("None"), QCalendarWidget::NoSelection);

    selectionModeLabel = new QLabel(tr("&Selection mode:"));
    selectionModeLabel->setBuddy(selectionModeCombo);

    gridCheckBox = new QCheckBox(tr("&Grid"));
    gridCheckBox->setChecked(calendar->isGridVisible());

    navigationCheckBox = new QCheckBox(tr("&Navigation bar"));
    navigationCheckBox->setChecked(true);

    horizontalHeaderCombo = new QComboBox;
    horizontalHeaderCombo->addItem(tr("Single letter day names"),
                                   QCalendarWidget::SingleLetterDayNames);
    horizontalHeaderCombo->addItem(tr("Short day names"),
                                   QCalendarWidget::ShortDayNames);
    horizontalHeaderCombo->addItem(tr("None"),
                                   QCalendarWidget::NoHorizontalHeader);
    horizontalHeaderCombo->setCurrentIndex(1);

    horizontalHeaderLabel = new QLabel(tr("&Horizontal header:"));
    horizontalHeaderLabel->setBuddy(horizontalHeaderCombo);

    verticalHeaderCombo = new QComboBox;
    verticalHeaderCombo->addItem(tr("ISO week numbers"),
                                 QCalendarWidget::ISOWeekNumbers);
    verticalHeaderCombo->addItem(tr("None"), QCalendarWidget::NoVerticalHeader);

    verticalHeaderLabel = new QLabel(tr("&Vertical header:"));
    verticalHeaderLabel->setBuddy(verticalHeaderCombo);

//! [11]
    connect(localeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Window::localeChanged);
    connect(firstDayCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Window::firstDayChanged);
    connect(selectionModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Window::selectionModeChanged);
    connect(gridCheckBox, &QCheckBox::toggled,
            calendar, &QCalendarWidget::setGridVisible);
    connect(navigationCheckBox, &QCheckBox::toggled,
            calendar, &QCalendarWidget::setNavigationBarVisible);
    connect(horizontalHeaderCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Window::horizontalHeaderChanged);
    connect(verticalHeaderCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Window::verticalHeaderChanged);
//! [11]

    QHBoxLayout *checkBoxLayout = new QHBoxLayout;
    checkBoxLayout->addWidget(gridCheckBox);
    checkBoxLayout->addStretch();
    checkBoxLayout->addWidget(navigationCheckBox);

    QGridLayout *outerLayout = new QGridLayout;
    outerLayout->addWidget(localeLabel, 0, 0);
    outerLayout->addWidget(localeCombo, 0, 1);
    outerLayout->addWidget(firstDayLabel, 1, 0);
    outerLayout->addWidget(firstDayCombo, 1, 1);
    outerLayout->addWidget(selectionModeLabel, 2, 0);
    outerLayout->addWidget(selectionModeCombo, 2, 1);
    outerLayout->addLayout(checkBoxLayout, 3, 0, 1, 2);
    outerLayout->addWidget(horizontalHeaderLabel, 4, 0);
    outerLayout->addWidget(horizontalHeaderCombo, 4, 1);
    outerLayout->addWidget(verticalHeaderLabel, 5, 0);
    outerLayout->addWidget(verticalHeaderCombo, 5, 1);
    generalOptionsGroupBox->setLayout(outerLayout);

//! [12]
    firstDayChanged(firstDayCombo->currentIndex());
    selectionModeChanged(selectionModeCombo->currentIndex());
    horizontalHeaderChanged(horizontalHeaderCombo->currentIndex());
    verticalHeaderChanged(verticalHeaderCombo->currentIndex());
}
//! [12]

//! [13]
void Window::createDatesGroupBox()
{
    datesGroupBox = new QGroupBox(tr("Dates"));

    minimumDateEdit = new QDateEdit;
    minimumDateEdit->setDisplayFormat("MMM d yyyy");
    minimumDateEdit->setDateRange(calendar->minimumDate(),
                                  calendar->maximumDate());
    minimumDateEdit->setDate(calendar->minimumDate());

    minimumDateLabel = new QLabel(tr("&Minimum Date:"));
    minimumDateLabel->setBuddy(minimumDateEdit);

    currentDateEdit = new QDateEdit;
    currentDateEdit->setDisplayFormat("MMM d yyyy");
    currentDateEdit->setDate(calendar->selectedDate());
    currentDateEdit->setDateRange(calendar->minimumDate(),
                                  calendar->maximumDate());

    currentDateLabel = new QLabel(tr("&Current Date:"));
    currentDateLabel->setBuddy(currentDateEdit);

    maximumDateEdit = new QDateEdit;
    maximumDateEdit->setDisplayFormat("MMM d yyyy");
    maximumDateEdit->setDateRange(calendar->minimumDate(),
                                  calendar->maximumDate());
    maximumDateEdit->setDate(calendar->maximumDate());

    maximumDateLabel = new QLabel(tr("Ma&ximum Date:"));
    maximumDateLabel->setBuddy(maximumDateEdit);

//! [13] //! [14]
    connect(currentDateEdit, &QDateEdit::dateChanged,
            calendar, &QCalendarWidget::setSelectedDate);
    connect(calendar, &QCalendarWidget::selectionChanged,
            this, &Window::selectedDateChanged);
    connect(minimumDateEdit, &QDateEdit::dateChanged,
            this, &Window::minimumDateChanged);
    connect(maximumDateEdit, &QDateEdit::dateChanged,
            this, &Window::maximumDateChanged);

//! [14]
    QGridLayout *dateBoxLayout = new QGridLayout;
    dateBoxLayout->addWidget(currentDateLabel, 1, 0);
    dateBoxLayout->addWidget(currentDateEdit, 1, 1);
    dateBoxLayout->addWidget(minimumDateLabel, 0, 0);
    dateBoxLayout->addWidget(minimumDateEdit, 0, 1);
    dateBoxLayout->addWidget(maximumDateLabel, 2, 0);
    dateBoxLayout->addWidget(maximumDateEdit, 2, 1);
    dateBoxLayout->setRowStretch(3, 1);

    datesGroupBox->setLayout(dateBoxLayout);
//! [15]
}
//! [15]

//! [16]

//CODICE PDS
void Window::createLabelTasks() {

    ToDoListView = new QListWidget();
    ToDoListView->setBackgroundRole(QPalette::Light);
    ToDoListView->setMinimumWidth(300);
    ToDoListView->setMinimumHeight(280);

    std::string currentDate = std::to_string(calendar->selectedDate().day()) + "/"
            + std::to_string(calendar->selectedDate().month()) + "/" + std::to_string(calendar->selectedDate().year());
    ToDoListData = new QListWidgetItem(currentDate.c_str());

    QFont titolo("Helvetica",16);
    ToDoListData->setFont(titolo);
    ToDoListView->addItem(ToDoListData);


    QListWidgetItem* p2 = new QListWidgetItem("stringa2");
    ToDoListView->addItem(p2);
}

//POLPETTA
void Window::createEditDeleteButtons() {

    QIcon icona(":/img/edit-img.png");

    if (icona.isNull()) {
        std::cout<<"Immagine "<<icona.name().toStdString()<<" presa"<<"\n";
    };

    QPixmap editImg(editImgChar);
    EditButton = new QPushButton(editImg, nullptr);
    //EditButton = new QPushButton(QIcon(":/edit-img.png"), nullptr);
    EditButton->setFixedWidth(70);
    EditButton->setFixedHeight(70);

    //:/edit-img.png

    DeleteButton = new QPushButton("Elimina", nullptr);
    DeleteButton->setFixedWidth(70);
    DeleteButton->setFixedHeight(70);

    //button->resize(50, 100);

    std::cout<<"createEditButton"<<"\n";



// add _lineEdit to your widget



}

void Window::createSearchBar() {

    SearchBar = new QLineEdit();
    SearchBar->setClearButtonEnabled(true);
    //SearchBar->addAction(":/resources/edit-img.png",QLineEdit::LeadingPosition);
    SearchBar->setPlaceholderText("Cerca...");
}


void Window::createTextFormatsGroupBox()
{
    textFormatsGroupBox = new QGroupBox(tr("Text Formats"));

    weekdayColorCombo = createColorComboBox();
    weekdayColorCombo->setCurrentIndex(
            weekdayColorCombo->findText(tr("Black")));

    weekdayColorLabel = new QLabel(tr("&Weekday color:"));
    weekdayColorLabel->setBuddy(weekdayColorCombo);

    weekendColorCombo = createColorComboBox();
    weekendColorCombo->setCurrentIndex(
            weekendColorCombo->findText(tr("Red")));

    weekendColorLabel = new QLabel(tr("Week&end color:"));
    weekendColorLabel->setBuddy(weekendColorCombo);

//! [16] //! [17]
    headerTextFormatCombo = new QComboBox;
    headerTextFormatCombo->addItem(tr("Bold"));
    headerTextFormatCombo->addItem(tr("Italic"));
    headerTextFormatCombo->addItem(tr("Plain"));

    headerTextFormatLabel = new QLabel(tr("&Header text:"));
    headerTextFormatLabel->setBuddy(headerTextFormatCombo);

    firstFridayCheckBox = new QCheckBox(tr("&First Friday in blue"));

    mayFirstCheckBox = new QCheckBox(tr("May &1 in red"));

//! [17] //! [18]
    connect(weekdayColorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Window::weekdayFormatChanged);
    connect(weekdayColorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Window::reformatCalendarPage);
    connect(weekendColorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Window::weekendFormatChanged);
    connect(weekendColorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Window::reformatCalendarPage);
    connect(headerTextFormatCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Window::reformatHeaders);
    connect(firstFridayCheckBox, &QCheckBox::toggled,
            this, &Window::reformatCalendarPage);
    connect(mayFirstCheckBox, &QCheckBox::toggled,
            this, &Window::reformatCalendarPage);

//! [18]
    QHBoxLayout *checkBoxLayout = new QHBoxLayout;
    checkBoxLayout->addWidget(firstFridayCheckBox);
    checkBoxLayout->addStretch();
    checkBoxLayout->addWidget(mayFirstCheckBox);

    QGridLayout *outerLayout = new QGridLayout;
    outerLayout->addWidget(weekdayColorLabel, 0, 0);
    outerLayout->addWidget(weekdayColorCombo, 0, 1);
    outerLayout->addWidget(weekendColorLabel, 1, 0);
    outerLayout->addWidget(weekendColorCombo, 1, 1);
    outerLayout->addWidget(headerTextFormatLabel, 2, 0);
    outerLayout->addWidget(headerTextFormatCombo, 2, 1);
    outerLayout->addLayout(checkBoxLayout, 3, 0, 1, 2);
    textFormatsGroupBox->setLayout(outerLayout);

    weekdayFormatChanged();
    weekendFormatChanged();
//! [19]
    reformatHeaders();
    reformatCalendarPage();
}
//! [19]

//! [20]

QComboBox *Window::createColorComboBox()
{
    QComboBox *comboBox = new QComboBox;
    comboBox->addItem(tr("Red"), QColor(Qt::red));
    comboBox->addItem(tr("Blue"), QColor(Qt::blue));
    comboBox->addItem(tr("Black"), QColor(Qt::black));
    comboBox->addItem(tr("Magenta"), QColor(Qt::magenta));
    return comboBox;
}
//! [20]

