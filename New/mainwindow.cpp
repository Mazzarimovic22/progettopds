#include "mainwindow.h"

MainWindow::MainWindow(QString username, QString password, QWidget *parent):
        QWidget(parent), username(username), password(password) {

    createPreviewGroupBox();
    createEditDeleteButtons();
    createSearchBar();

    //NEW
    QVBoxLayout* layout = new QVBoxLayout;

    QVBoxLayout* leftLayout = new QVBoxLayout;
    QVBoxLayout* rightLayout = new QVBoxLayout;
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    QHBoxLayout* centralLayout = new QHBoxLayout;

    //TODOLIST
    QVBoxLayout* eventLayout = new QVBoxLayout;
    eventLayout->setSpacing(0);


    QLabel* l1 = new QLabel("l1");
    l1->setStyleSheet("background-color: white;");
    QLabel* l2 = new QLabel("l2");
    l2->setStyleSheet("background-color: white;");
    QLabel* l3 = new QLabel("l3");
    l3->setStyleSheet("background-color: white;");
    QLabel* l4 = new QLabel("l4");
    l4->setStyleSheet("background-color: white;");

    QVBoxLayout* eventInsideLeftLayout = new QVBoxLayout;
    eventInsideLeftLayout->addWidget(l1);
    eventInsideLeftLayout->addWidget(l2);

    QVBoxLayout* eventInsideRightLayout = new QVBoxLayout;
    eventInsideRightLayout->addWidget(l3);
    eventInsideRightLayout->addWidget(l4);

    QHBoxLayout* eventInsideCentralLayout = new QHBoxLayout;
    eventInsideCentralLayout->addLayout(eventInsideLeftLayout);
    eventInsideCentralLayout->addLayout(eventInsideRightLayout);


    eventLayout->addLayout(eventInsideCentralLayout);

    leftLayout->addWidget(SearchBar);
    leftLayout->addLayout(eventLayout);
    leftLayout->addStretch();

    rightLayout->addWidget(calendar);

    buttonLayout->addWidget(EditButton);
    buttonLayout->addWidget(DeleteButton);

    centralLayout->addLayout(leftLayout);
    centralLayout->addLayout(rightLayout);

    layout->addLayout(centralLayout);
    layout->addLayout(buttonLayout);

    setLayout(layout);
    setWindowTitle(tr("Main Window"));
    setFixedHeight(sizeHint().height());
}


void MainWindow::createPreviewGroupBox()
{

    calendar = new QCalendarWidget;
    calendar->setMinimumHeight(280);
    calendar->setMinimumHeight(300);

    calendar->setMinimumDate(QDate(1900, 1, 1));
    calendar->setMaximumDate(QDate(3000, 1, 1));
    calendar->setGridVisible(true);

    connect(calendar, &QCalendarWidget::currentPageChanged,
            this, &MainWindow::reformatCalendarPage);

    previewLayout = new QGridLayout;
    previewLayout->addWidget(calendar, 0, 0, Qt::AlignCenter);

    /*previewGroupBox->setLayout(previewLayout);
    */
}

void MainWindow::localeChanged(int index)
{
    const QLocale newLocale(localeCombo->itemData(index).toLocale());
    calendar->setLocale(newLocale);
    int newLocaleFirstDayIndex = firstDayCombo->findData(newLocale.firstDayOfWeek());
    firstDayCombo->setCurrentIndex(newLocaleFirstDayIndex);
}

void MainWindow::firstDayChanged(int index)
{
    calendar->setFirstDayOfWeek(Qt::DayOfWeek(
            firstDayCombo->itemData(index).toInt()));
}

void MainWindow::selectionModeChanged(int index)
{
    calendar->setSelectionMode(QCalendarWidget::SelectionMode(
            selectionModeCombo->itemData(index).toInt()));
}

void MainWindow::horizontalHeaderChanged(int index)
{
    calendar->setHorizontalHeaderFormat(QCalendarWidget::HorizontalHeaderFormat(
            horizontalHeaderCombo->itemData(index).toInt()));
}

void MainWindow::verticalHeaderChanged(int index)
{
    calendar->setVerticalHeaderFormat(QCalendarWidget::VerticalHeaderFormat(
            verticalHeaderCombo->itemData(index).toInt()));
}

void MainWindow::selectedDateChanged()
{
    currentDateEdit->setDate(calendar->selectedDate());
    std::string currentDate = std::to_string(calendar->selectedDate().day()) + "/"
                              + std::to_string(calendar->selectedDate().month()) + "/" + std::to_string(calendar->selectedDate().year());

    //ToDoListData->setText(currentDate.c_str());
}

void MainWindow::minimumDateChanged(QDate date)
{
    calendar->setMinimumDate(date);
    maximumDateEdit->setDate(calendar->maximumDate());
}

void MainWindow::maximumDateChanged(QDate date)
{
    calendar->setMaximumDate(date);
    minimumDateEdit->setDate(calendar->minimumDate());
}

void MainWindow::weekdayFormatChanged()
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

void MainWindow::weekendFormatChanged()
{
    QTextCharFormat format;

    format.setForeground(qvariant_cast<QColor>(
            weekendColorCombo->itemData(weekendColorCombo->currentIndex())));
    calendar->setWeekdayTextFormat(Qt::Saturday, format);
    calendar->setWeekdayTextFormat(Qt::Sunday, format);
}

void MainWindow::reformatHeaders()
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

void MainWindow::reformatCalendarPage()
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

void MainWindow::createEditDeleteButtons() {

    EditButton = new QPushButton(nullptr);
    EditButton->setIcon(QIcon(":/flat-edit.png"));
    EditButton->setFixedWidth(40);
    EditButton->setFixedHeight(40);
    EditButton->setIconSize(QSize(40,40));
    EditButton->setStyleSheet("border: none;");

    connect(EditButton, SIGNAL(clicked()), this, SLOT(editClicked()));

    DeleteButton = new QPushButton(nullptr);
    DeleteButton->setIcon(QIcon(":/flat-trash.png"));
    DeleteButton->setFixedWidth(40);
    DeleteButton->setFixedHeight(40);
    DeleteButton->setIconSize(QSize(40,40));
    DeleteButton->setStyleSheet("border: none;");
    connect(DeleteButton, SIGNAL(clicked()), this, SLOT(deleteClicked()));
}

void MainWindow::createSearchBar() {

    SearchBar = new QLineEdit();
    SearchBar->setStyleSheet("border: none;");
    SearchBar->setClearButtonEnabled(true);
    SearchBar->setPlaceholderText("Cerca...");
}

void MainWindow::editClicked() {

    //editWindow = new EditWindow(username, password, calendar->selectedDate(), manager);
    //editWindow->show();

    //createCalendarObject();
}

void MainWindow::deleteClicked() {

}


