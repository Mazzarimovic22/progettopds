#include "window.h"


Window::Window(QString username, QString password, QWidget *parent):
    QWidget(parent), username(username), password(password){

    //Network Manager
    manager = new QNetworkAccessManager(this);

    //Calendario
    createPreviewGroupBox();
    createGeneralOptionsGroupBox();
    createDatesGroupBox();
    createTextFormatsGroupBox();

    //ToDoList
    createLabelTasks();
    createEditDeleteButtons();
    createSearchBar();

    //Layout
    QGridLayout *layout = new QGridLayout;

    layout->addWidget(SearchBar,0,0);
    layout->addWidget(toDoList,1,0);
    //layout->addWidget(calendar, 1, 1);
    //layout->addWidget(EditButton,2,0);
    //layout->addWidget(DeleteButton,2,1);

    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);

    previewLayout->setRowMinimumHeight(0, calendar->sizeHint().height());
    previewLayout->setColumnMinimumWidth(0, calendar->sizeHint().width());

    setWindowTitle(tr("Calendario e To-Do List"));
}

QString Window::encodeBase64(QString string){
  QByteArray ba;
  ba.append(string.toStdString());
  return ba.toBase64();
}

void Window::createCalendarObject(){
    QUrl url("http://localhost/calendarserver.php/calendars/");
    url.setUserName(username);
    url.setPassword(password);

    QString uid("132456-34365");

    QString requestString = "BEGIN:VCALENDAR\r\n"
                            "BEGIN:VEVENT\r\n"
                            "UID:" + uid + "\r\n"
                            "VERSION:2.0\r\n"
                            "DTSTAMP:" + QDateTime::currentDateTime().toString("yyyyMMddTHHmmssZ") + "\r\n"
                            "SUMMARY:" + "Weekly meeting" + "\r\n"
                            "DTSTART:" + "20120101T120000" + "\r\n"
                            //"DTEND:" + endDateTime.toString("yyyyMMddTHHmmss") + "\r\n"
                            //"LOCATION:" + location + "\r\n"
                            //"DESCRIPTION:" + description + "\r\n"
                            "TRANSP:OPAQUE\r\n";

      requestString.append("END:VEVENT\r\nEND:VCALENDAR");

      QNetworkRequest request=QNetworkRequest(url);
      QByteArray *dataToSend = new QByteArray(requestString.toLatin1());
      request.setHeader(QNetworkRequest::ContentLengthHeader, dataToSend->size());
      QBuffer *buffer = new QBuffer(dataToSend);

      buffer->open(QIODevice::ReadWrite);

      int buffersize = buffer->write(requestString.toUtf8());
      buffer->seek(0);
      buffer->size();

      QByteArray contentlength;
      contentlength.append(QString::number(buffersize).toStdString());

      request.setUrl(QUrl(url.toString() + username + "/default/prova.ics"));
      request.setRawHeader("Content-Type", "text/calendar; charset=utf-8");
      request.setRawHeader("Content-Length", contentlength);

      manager->put(request, buffer);

      connect(manager,&QNetworkAccessManager::finished, [](QNetworkReply *reply){
        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        qDebug() << reply->readAll();
      });
}

void Window::getCalendar(){
    QUrl url("http://localhost/calendarserver.php/calendars/");
    url.setUserName(username);
    url.setPassword(password);

    QString requestString = "<c:calendar-query xmlns:d=\"DAV:\" xmlns:c=\"urn:ietf:params:xml:ns:caldav\">\r\n"
    "    <d:prop>\r\n"
    "        <d:getetag />\r\n"
    "        <c:calendar-data />\r\n"
    "    </d:prop>\r\n"
    "    <c:filter>\r\n"
    "        <c:comp-filter name=\"VCALENDAR\" />\r\n"
    "    </c:filter>\r\n"
    "</c:calendar-query>";

    QNetworkRequest request=QNetworkRequest(url);
    QByteArray *dataToSend = new QByteArray(requestString.toLatin1());
    request.setHeader(QNetworkRequest::ContentLengthHeader, dataToSend->size());
    QBuffer *buffer = new QBuffer(dataToSend);

    buffer->open(QIODevice::ReadWrite);

    int buffersize = buffer->write(requestString.toUtf8());
    buffer->seek(0);
    buffer->size();

    QByteArray contentlength;
    contentlength.append(QString::number(buffersize).toStdString());

    request.setUrl(QUrl(url.toString() + username + "/default/"));
    request.setRawHeader("User-Agent", "PostmanRuntime/7.28.4");
    request.setRawHeader("Depth", "1");
    request.setRawHeader("Prefer", "return-minimal");
    request.setRawHeader("Content-Type", "application/xml; charset=utf-8");
    request.setRawHeader("Content-Length", contentlength);

    manager->sendCustomRequest(request, "REPORT", buffer);

    connect(manager, &QNetworkAccessManager::finished, [this](QNetworkReply *reply){

            QDomDocument doc;

            doc.setContent(reply);

            QDomNodeList list_response = doc.elementsByTagName("d:response");
            for (int i = 0; i < list_response.size(); i++){
                 QDomNode thisResponse = list_response.item(i);

                 QDomNode thisPropStat = thisResponse.firstChildElement("d:propstat");
                 QDomElement elPropStatus = thisPropStat.firstChildElement("d:status");
                 //qDebug() << "    PROPSTATUS = " << elPropStatus.text();

                 QDomNode thisProp = thisPropStat.firstChildElement("d:prop");
                 QDomElement elETag = thisProp.firstChildElement("d:getetag");
                 //qDebug() << "    ETAG = " << elETag.text();

                 QDomElement elCalendarData = thisProp.firstChildElement("cal:calendar-data");
                 //qDebug() << " CALENDARDATA = " << elCalendarData.text().toLatin1();

                 QStringList prova = elCalendarData.text().split('\n');

                 QString summary = "";
                 QString location = "";
                 QString dtstart = "";
                 QString dtend = "";

                  for(int j = 0; j < prova.size(); j++) {

                    if(prova.at(j).startsWith("SUMMARY")) {
                      summary = prova.at(j).split(":").last();
                    }

                    if(prova.at(j).startsWith("LOCATION")) {
                      location = prova.at(j).split(":").last();
                    }

                    if(prova.at(j).startsWith("DTSTART")) {
                      dtstart = prova.at(j).split(":").last();
                    }

                    if(prova.at(j).startsWith("DTEND")) {
                      dtend = prova.at(j).split(":").last();
                    }
                  }
                   this->addToView(summary,location,dtstart, dtend);
        }
        });
}
//------------------------------------
//CALENDARIO
void Window::localeChanged(int index){
    const QLocale newLocale(localeCombo->itemData(index).toLocale());
    calendar->setLocale(newLocale);
    int newLocaleFirstDayIndex = firstDayCombo->findData(newLocale.firstDayOfWeek());
    firstDayCombo->setCurrentIndex(newLocaleFirstDayIndex);
}

void Window::firstDayChanged(int index){
    calendar->setFirstDayOfWeek(Qt::DayOfWeek(
            firstDayCombo->itemData(index).toInt()));
}

void Window::selectionModeChanged(int index){
    calendar->setSelectionMode(QCalendarWidget::SelectionMode(
            selectionModeCombo->itemData(index).toInt()));
}

void Window::horizontalHeaderChanged(int index){
    calendar->setHorizontalHeaderFormat(QCalendarWidget::HorizontalHeaderFormat(
            horizontalHeaderCombo->itemData(index).toInt()));
}

void Window::verticalHeaderChanged(int index){
    calendar->setVerticalHeaderFormat(QCalendarWidget::VerticalHeaderFormat(
            verticalHeaderCombo->itemData(index).toInt()));
}

void Window::selectedDateChanged(){
    currentDateEdit->setDate(calendar->selectedDate());
    std::string currentDate = std::to_string(calendar->selectedDate().day()) + "/"
                              + std::to_string(calendar->selectedDate().month()) + "/" + std::to_string(calendar->selectedDate().year());

    ToDoListData->setText(currentDate.c_str());
}

void Window::minimumDateChanged(QDate date){
    calendar->setMinimumDate(date);
    maximumDateEdit->setDate(calendar->maximumDate());
}

void Window::maximumDateChanged(QDate date){
    calendar->setMaximumDate(date);
    minimumDateEdit->setDate(calendar->minimumDate());
}

void Window::weekdayFormatChanged(){
    QTextCharFormat format;

    format.setForeground(qvariant_cast<QColor>(
    weekdayColorCombo->itemData(weekdayColorCombo->currentIndex())));
    calendar->setWeekdayTextFormat(Qt::Monday, format);
    calendar->setWeekdayTextFormat(Qt::Tuesday, format);
    calendar->setWeekdayTextFormat(Qt::Wednesday, format);
    calendar->setWeekdayTextFormat(Qt::Thursday, format);
    calendar->setWeekdayTextFormat(Qt::Friday, format);
}

void Window::weekendFormatChanged(){
    QTextCharFormat format;

    format.setForeground(qvariant_cast<QColor>(
            weekendColorCombo->itemData(weekendColorCombo->currentIndex())));
    calendar->setWeekdayTextFormat(Qt::Saturday, format);
    calendar->setWeekdayTextFormat(Qt::Sunday, format);
}

void Window::reformatHeaders(){
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

void Window::reformatCalendarPage(){
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

//Creazione Calendario
void Window::createPreviewGroupBox(){
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
}

void Window::createGeneralOptionsGroupBox(){
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

    firstDayChanged(firstDayCombo->currentIndex());
    selectionModeChanged(selectionModeCombo->currentIndex());
    horizontalHeaderChanged(horizontalHeaderCombo->currentIndex());
    verticalHeaderChanged(verticalHeaderCombo->currentIndex());
}

void Window::createDatesGroupBox(){
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

    connect(currentDateEdit, &QDateEdit::dateChanged,
            calendar, &QCalendarWidget::setSelectedDate);
    connect(calendar, &QCalendarWidget::selectionChanged,
            this, &Window::selectedDateChanged);
    connect(minimumDateEdit, &QDateEdit::dateChanged,
            this, &Window::minimumDateChanged);
    connect(maximumDateEdit, &QDateEdit::dateChanged,
            this, &Window::maximumDateChanged);

    QGridLayout *dateBoxLayout = new QGridLayout;
    dateBoxLayout->addWidget(currentDateLabel, 1, 0);
    dateBoxLayout->addWidget(currentDateEdit, 1, 1);
    dateBoxLayout->addWidget(minimumDateLabel, 0, 0);
    dateBoxLayout->addWidget(minimumDateEdit, 0, 1);
    dateBoxLayout->addWidget(maximumDateLabel, 2, 0);
    dateBoxLayout->addWidget(maximumDateEdit, 2, 1);
    dateBoxLayout->setRowStretch(3, 1);

    datesGroupBox->setLayout(dateBoxLayout);
}

void Window::createTextFormatsGroupBox(){
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

    headerTextFormatCombo = new QComboBox;
    headerTextFormatCombo->addItem(tr("Bold"));
    headerTextFormatCombo->addItem(tr("Italic"));
    headerTextFormatCombo->addItem(tr("Plain"));

    headerTextFormatLabel = new QLabel(tr("&Header text:"));
    headerTextFormatLabel->setBuddy(headerTextFormatCombo);

    firstFridayCheckBox = new QCheckBox(tr("&First Friday in blue"));

    mayFirstCheckBox = new QCheckBox(tr("May &1 in red"));

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
    reformatHeaders();
    reformatCalendarPage();
}

QComboBox *Window::createColorComboBox(){
    QComboBox *comboBox = new QComboBox;
    comboBox->addItem(tr("Red"), QColor(Qt::red));
    comboBox->addItem(tr("Blue"), QColor(Qt::blue));
    comboBox->addItem(tr("Black"), QColor(Qt::black));
    comboBox->addItem(tr("Magenta"), QColor(Qt::magenta));
    return comboBox;
}

//------------------------------------
//TO-DO LIST
void Window::createLabelTasks(){

    /*ToDoListView = new QListWidget();
    ToDoListView->setStyleSheet("border: none;");
    ToDoListView->setBackgroundRole(QPalette::Light);
    ToDoListView->setMinimumWidth(300);
    ToDoListView->setMinimumHeight(280);

    //NEW TO-DO LIST
    //------------------------------
    //Lays out horizontally instead of vertically
    ToDoListView->setFlow(QListView::TopToBottom);

    //Dynamically adjust contents
    ToDoListView->setResizeMode(QListView::Adjust);

    //This is an arbitrary value, but it forces the layout into a grid
    ToDoListView->setGridSize(QSize(280, 64));
    //And the most important part:
    //ToDoListView->setViewMode(QListView::IconMode);*/
    //------------------------------

    //------------------------------
    //DAY SELECTED
    std::string currentDate = std::to_string(calendar->selectedDate().day()) + "/"
            + std::to_string(calendar->selectedDate().month()) + "/" + std::to_string(calendar->selectedDate().year());
    ToDoListData = new QListWidgetItem(currentDate.c_str());

    QFont titolo("Helvetica",16);
    ToDoListData->setFont(titolo);
    //ToDoListView->addItem(ToDoListData);
    //------------------------------

    toDoList = new ToDoApps(username, password);
}

//Inserimento items nella To-Do List
void Window::addToView(QString summary, QString location, QString dtstart, QString dtend){

    QLabel* l1 = new QLabel(summary);
    l1->setStyleSheet("background-color: white;");
    QLabel* l2 = new QLabel(location);
    l2->setStyleSheet("background-color: white;");
    QLabel* l3 = new QLabel(dtstart);
    l3->setStyleSheet("background-color: white;");
    QLabel* l4 = new QLabel(dtend);
    l4->setStyleSheet("background-color: white;");

    qDebug() <<"ADD TO VIEW";
}

//------------------------------------
//EDIT E DELETE BUTTONS
void Window::createEditDeleteButtons(){

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

void Window::editClicked(){

    editWindow = new EditWindow(username, password, calendar->selectedDate(), manager);
    editWindow->show();

    //createCalendarObject();
}

void Window::deleteClicked(){

}

//------------------------------------
//SEARCH BAR
void Window::createSearchBar(){

    SearchBar = new QLineEdit();
    SearchBar->setStyleSheet("border: none;");
    SearchBar->setClearButtonEnabled(true);
    //SearchBar->addAction(":/resources/edit-img.png",QLineEdit::LeadingPosition);
    SearchBar->setPlaceholderText("Cerca...");
}


