#include "todoapps.h"

ToDoApps::ToDoApps(QString username, QString password, QWidget *parent)
    : QWidget(parent), username(username), password(password){
    ui.setupUi(this);

    // Set stylesheet
    initStylesheet();

    //Network Manager
    manager = new QNetworkAccessManager(this);

    events = new std::vector<Event>;

    //ADD BUTTON STYLE
    ui.AddNewBtn->setText("");
    ui.AddNewBtn->setIcon(QIcon(":/add.png"));
    ui.AddNewBtn->setFixedWidth(40);
    ui.AddNewBtn->setFixedHeight(40);
    ui.AddNewBtn->setIconSize(QSize(40,40));
    ui.AddNewBtn->setStyleSheet("border: none;");

    // Connect the add new task button
    connect(ui.AddNewBtn, SIGNAL(clicked()), this, SLOT(SlotAddNewTask()));

    ui.NewTask->deleteLater(); // this is just for reference before i did the code

    ui.NewTaskLineEdit->setText("Today, I want to...");
    ui.NewTaskLocationEdit->setText("Insert a location...");
    ui.NewDateText->setText(QDate::currentDate().toString());

    this->getCalendar();

}


void ToDoApps::initStylesheet(){

    // You have to create the css file first in the main project directory

    // Get the css file
    QFile style("style_Generic.css");
    bool ok = style.open(QFile::ReadOnly);
    QString s = QString::fromLatin1(style.readAll());

    setStyleSheet(s);

}


void ToDoApps::SlotAddNewTask(){

    //------------------------------------
    //Creation NEW TASK

    //Get SUMMARY
    QString taskName = ui.NewTaskLineEdit->text();

    //Get LOCATION
    QString location = ui.NewTaskLocationEdit->text();

    //Get current DATE
    //QString date = QDate::currentDate().toString();
    QString date = ui.calendarWidget->selectedDate().toString();

    //GET START TIME and END TIME
    QTime dtstart = ui.startTimeEdit->time();
    QTime dtend = ui.endTimeEdit->time();

    addEventRequest(taskName, date, location, dtstart, dtend);

    //Reset interface to default placeholder
    ui.NewTaskLineEdit->setText("Today, I want to...");
    ui.NewTaskLocationEdit->setText("Insert a location...");
    ui.startTimeEdit->setTime(QTime(0,0));
    ui.endTimeEdit->setTime(QTime(0,0));
    //------------------------------------

    // This is how to trigger signal
    //emit SignalAddNewTask(taskName, date);

    // Then in other class/widget connect it like
    //ToDoApps* obj;
    //connect(obj, SIGNAL(SignalAddNewTask(QString, QString)), this, SLOT(SlotToSomethingInThatClass()));

}


// This is the part where widget created using code / the reference is in .ui file
void ToDoApps::createNewTask(QString uid, QString taskName, QString date, QString location, QString dtstart, QString dtend){

    {
        // The hierarchy of this widget will be like this
        /*
            + Hframe (Frame)
                + Vframe (Frame)
                    + titlelabel (Label)
                    + tasklabel (Label)
                    + datelabel (Label)
                + spacer (Horizontal spacer)
                + deleteBtn (PushButton)
        */
    }

    // Get the parent widget which the widget created to be child in
    QVBoxLayout* vMainLayout = qobject_cast<QVBoxLayout*>(ui.AllNewTasksContents->layout());

    // Create Frame for the main widget container
    QFrame* Hframe = new QFrame();
    Hframe->setFrameStyle(QFrame::StyledPanel);
    // Create Horizontal Box Layout as the Frame layout and also for easily add widget inside it
    // This is like a virtual layout. It doesn't added into ui as a ui things bcos it is just a layout.
    QHBoxLayout* newTask = new QHBoxLayout(Hframe);
    Hframe->setLayout(newTask);



    // Create Frame for the details container; task title, task name, task date created
    QFrame* Vframe = new QFrame();
    QVBoxLayout* taskDetails = new QVBoxLayout(Vframe);
    Vframe->setLayout(taskDetails);

    //------------------------------------
    //TASK INFO

    //Task NUMBER
    QLabel* titlelabel = new QLabel(tr("Task #%1").arg(vMainLayout->count()));
    taskDetails->addWidget(titlelabel);

    //Add SUMMARY
    QLabel* tasklabel = new QLabel(taskName);
    taskDetails->addWidget(tasklabel);

    //Add DATE
    QLabel* datelabel = new QLabel(date);
    taskDetails->addWidget(datelabel);

    //Add LOCATION
    QLabel* locationlabel = new QLabel(location);
    taskDetails->addWidget(locationlabel);

    //Add TIME
    QLabel* dtstartlabel = new QLabel(dtstart);
    taskDetails->addWidget(dtstartlabel);

    QLabel* dtendlabel = new QLabel(dtend);
    taskDetails->addWidget(dtendlabel);

    //------------------------------------

    // Insert the task details frame inside main task box layout
    newTask->insertWidget(0, Vframe);

    // Insert horizontal spacer in between Vframe and deleteBtn
    QSpacerItem* spacer = new QSpacerItem(100, 100, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
    newTask->insertSpacerItem(1, spacer);

    //DELETE BUTTON
    QPushButton* deleteBtn = new QPushButton();
    deleteBtn->setIcon(QIcon(":/flat-trash.png"));
    deleteBtn->setFixedWidth(40);
    deleteBtn->setFixedHeight(40);
    deleteBtn->setIconSize(QSize(40,40));
    deleteBtn->setStyleSheet("border: none;");
    newTask->insertWidget(2, deleteBtn);

    // Store the current duplicated widget to the delete button property for easy reference
    // This is a useful function if you want to reference any widgets to the specific button
    deleteBtn->setProperty("CurrentTask", QVariant(QVariant::fromValue<QFrame*>(Hframe)));

    // Insert into parent ui frame
    vMainLayout->insertWidget(vMainLayout->count()-1, Hframe);

    // Connect the delete button
    connect(deleteBtn, SIGNAL(clicked()), this, SLOT(SlotDeleteTask()));


    // This part is how you can set stylesheet in code
    {
        QVector<QString> colors = { "rgba(66, 165, 245,1.0)", "rgba(41, 182, 246,1.0)", "rgba(38, 198, 218,1.0)", "rgba(38, 166, 154,1.0)", "rgba(102, 187, 106,1.0)", "rgba(156, 204, 101,1.0)", "rgba(212, 225, 87,1.0)", "rgba(255, 238, 88,1.0)", "rgba(255, 202, 40,1.0)", "rgba(255, 167, 38,1.0)"};
        int randomVal = (((rand() % 50) % 100) % (colors.size() - 1));

        //serve settare l'uid come object name in modo da poter fare la delete sul db
        Hframe->setObjectName(uid);
        Hframe->setStyleSheet("#NewTask { border-radius: 10px; border: 1px solid black; background-color: " + colors[randomVal] + "; }");

        tasklabel->setObjectName("TaskName");
        tasklabel->setStyleSheet("#TaskName { font: bold 11pt black 'Verdana'; }");

        deleteBtn->setObjectName("DeleteBtn");
        deleteBtn->setStyleSheet("#DeleteBtn { color: white; background-color: #ff2264; border-color: #b91043; }  #DeleteBtn:hover{ background-color: #b91043; border-color: #ff2264; }");
    }
}


void ToDoApps::SlotDeleteTask(){

    // Get the sender widget
    QPushButton* fromButton = (QPushButton*)sender();

    // Get the widget referenced in the property
    QVariant var;
    var = fromButton->property("CurrentTask");
    eventToDelete = qvariant_cast<QFrame*>(var);

    //removeEventRequest();

    QString uid = eventToDelete->objectName();

    uidToDelete = uid;

    removeEventRequest(uid);

}

void ToDoApps::getCalendar() {
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
                 QString date = "";
                 QString dtstart = "";
                 QString dtend = "";
                 QString uid = "";

                  for(int j = 0; j < prova.size(); j++) {

                    if(prova.at(j).startsWith("SUMMARY")) {
                      summary = prova.at(j).split(":").last();
                    }

                    if(prova.at(j).startsWith("LOCATION")) {
                      location = prova.at(j).split(":").last();
                    }

                    if(prova.at(j).startsWith("DTSTART")) {
                      date = prova.at(j).split(":").last().split("T").first();
                    }

                    if(prova.at(j).startsWith("DTSTART")) {
                      dtstart = prova.at(j).split(":").last();
                    }

                    if(prova.at(j).startsWith("DTEND")) {
                      dtend = prova.at(j).split(":").last();
                    }

                    if(prova.at(j).startsWith("UID")) {
                      uid = prova.at(j).split(":").last();
                    }
                  }

                  //qDebug() << "DATE = " << date << " SELECTED DATE = " << ui.calendarWidget->selectedDate().toString("yyyyMMdd") <<"\n";

                  //if(date == ui.calendarWidget->selectedDate().toString("yyyyMMdd")){
                   //this->createNewTask(summary,ui.calendarWidget->selectedDate().toString(),location,dtstart, dtend);

                    Event e(uid, summary,location,date,dtstart, dtend);
                    events->push_back(e);
                  //}
            }
            this->showCurrentEvents();
        });
}

void ToDoApps::on_calendarWidget_clicked(const QDate &date){
    //La data selezionata del calendario modifica quella del task
    ui.NewDateText->setText(date.toString());

   // qDebug() << events->size();
    showCurrentEvents();

}

void ToDoApps::showCurrentEvents(){
    //Delete events previously showed
    QVBoxLayout* vMainLayout = qobject_cast<QVBoxLayout*>(ui.AllNewTasksContents->layout());

    //qua ci vuole un -1 perchè il count è 1 nel caso in cui non ci sia nulla nel layout. Evita un null pointer
    for(int i = 0; i < vMainLayout->count() -1; i++) {
        vMainLayout->itemAt(i)->widget()->deleteLater();
        //qDebug() << "Delete item\n";
     }
    for(auto event: *events) {
        if(event.date == ui.calendarWidget->selectedDate().toString("yyyyMMdd")){
            this->createNewTask(event.uid,event.summary,event.location, ui.calendarWidget->selectedDate().toString(),event.dtstart, event.dtend);
            //qDebug() << event.date;
        }
    }
}

void ToDoApps::addEventRequest(QString taskName, QString date, QString location, QTime dtstart, QTime dtend){

    //TO-DO: CONTROLLARE CHE START SIA < DI END
    //qDebug() << taskName <<"\t"<< date <<"\t"<< location <<"\t"<< dtstart <<"\t"<< dtend <<"\n";


    QUrl url("http://localhost/calendarserver.php/calendars/");
    url.setUserName(username);
    url.setPassword(password);

    QDateTime startDateTime;
    QDateTime endDateTime;

    startDateTime.setDate(ui.calendarWidget->selectedDate());
    startDateTime.setTime(dtstart);

    endDateTime.setDate(ui.calendarWidget->selectedDate());
    endDateTime.setTime(dtend);

    //è necessario cambiare l'uid perchè non è buona norma mettere il taskname (che contiene spazi)
    //questo potrebbe causare errori nel caso della delete
    //QString uid = QDateTime::currentDateTime().toString("yyyyMMdd-HHMM-00ss") + "-0000-" +  taskName;
    QString uid = QDateTime::currentDateTime().toString("yyyyMMdd-HHMM-00ss") + "-0000-" + startDateTime.toString("yyyyMMddHHMM");


    QString requestString = "BEGIN:VCALENDAR\r\n"
                            "BEGIN:VEVENT\r\n"
                            "UID:" + uid + "\r\n"
                            "VERSION:2.0\r\n"

                            "SUMMARY:" + taskName + "\r\n"
                            "LOCATION:" + location + "\r\n"
                            "DTSTART:" + startDateTime.toString("yyyyMMddTHHmmss") + "\r\n"
                            "DTEND:" + endDateTime.toString("yyyyMMddTHHmmss") + "\r\n"

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

  request.setUrl(QUrl(url.toString() + username + "/default/" + uid + ".ics"));
  request.setRawHeader("Content-Type", "text/calendar; charset=utf-8");
  request.setRawHeader("Content-Length", contentlength);

  //creo localmente l'evento che, se la richiesta andrà a buon fine, sarà visualizzato
  QString date2 = startDateTime.toString("yyyyMMddTHHmmss").split('T').first();
  eventToPut = new Event(uid, taskName, location, date2, startDateTime.toString("yyyyMMddTHHmmss"), endDateTime.toString("yyyyMMddTHHmmss"));

  putReply = manager->put(request, buffer);

  /*
  connect(manager,&QNetworkAccessManager::finished, [this, uid, taskName, location, startDateTime, endDateTime](QNetworkReply *reply){
    if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 201) {
        qDebug() << "reply to add executed";
        QString date = startDateTime.toString("yyyyMMddTHHmmss").split('T').first();
        Event e(uid, taskName, location, date, startDateTime.toString("yyyyMMddTHHmmss"), endDateTime.toString("yyyyMMddTHHmmss"));
        events->push_back(e);
        //stampo events
        qDebug() << "\n\nSTAMPA DI EVENTS";
        for(auto event: *events) {
           qDebug() << event.uid;
        }
        this->showCurrentEvents();
    }
    else {
        QMessageBox::information(this, "Error", "NETWORK ERROR!");
    }
    reply->deleteLater();
  });*/

  if(NULL != putReply){
      //connect(putReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleError()));
      connect(putReply, SIGNAL(finished()), this, SLOT(addEventRequestFinished()));
  }
}

void ToDoApps::addEventRequestFinished(){
    if(NULL != putReply){
        if(putReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 201) {
            qDebug() << "reply to add executed";
            events->push_back(*eventToPut);
            /*stampo events
            qDebug() << "\n\nSTAMPA DI EVENTS";
            for(auto event: *events) {
               qDebug() << event.uid;
            }*/
            this->showCurrentEvents();
        }
        else {
            QMessageBox::information(this, "Error", "NETWORK ERROR!");
        }
        putReply->deleteLater();
    }
}

void ToDoApps::handleError(){
    if(NULL != putReply){
        QMessageBox::information(this, "Error", "NETWORK ERROR!");
        putReply->deleteLater();
    }
}

void ToDoApps::removeEventRequest(QString uid){
    QUrl url("http://localhost/calendarserver.php/calendars/");
    url.setUserName(username);
    url.setPassword(password);

    QString filename = uid + ".ics";


//    QString filename = QUrl(href).fileName();

    QNetworkRequest request;
    request.setUrl(QUrl(url.toString() + username + "/default/" + filename));
    request.setRawHeader("Content-Type", "text/calendar; charset=utf-8");
    request.setRawHeader("Content-Length", 0);


    qDebug() << "deleting .. " << request.url();

    deleteReply = manager->deleteResource(request);

    if(NULL != deleteReply){
        connect(deleteReply, SIGNAL(finished()), this, SLOT(removeEventRequestFinished()));
    }

    /*connect(manager,&QNetworkAccessManager::finished, [this](QNetworkReply *reply){

//      if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 201) {
//          this->showCurrentEvents();
//      }
//      else {
//          QMessageBox::information(this, "Error", "NETWORK ERROR!");
//      }
      qDebug() << reply->readAll();
      reply->deleteLater();
    });*/

}

void ToDoApps::removeEventRequestFinished(){

    //elimino l'evento dal vettore di eventi
    int i=0;
    for(auto event: *events) {
        if(event.uid == uidToDelete){
            events->erase(events->begin()+i);
            break;
        }
        i++;
    }
    //elimino effettivamente il task dalla schermata solo se la richiesta è andata a buon fine
    eventToDelete->deleteLater();
    delete eventToDelete;
    this->showCurrentEvents();

    qDebug() << deleteReply->readAll();
    deleteReply->deleteLater();
}

void ToDoApps::on_AddNewBtn_clicked(){
//   qDebug() << "Add clicked" << "\n";
//   addEventRequest();

}

void ToDoApps::on_DeleteTask_clicked(){
//   qDebug() << "Add clicked" << "\n";
//   addEventRequest();

}
