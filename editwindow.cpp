#include "editwindow.h"
#include "todoapps.h"

EditWindow::EditWindow(QString username, QString password, Event event, QNetworkAccessManager* manager, QWidget *parent):
    QWidget(parent), username(username), password(password), event(event), manager(manager){

    day = new QLabel();
    day->setText(QDate::fromString(event.date, "yyyyMMdd").toString());
    day->setStyleSheet("font-size: 20px;");

    summary = new QLabel(tr("Summary:"));
    location = new QLabel(tr("Location:"));
    start = new QLabel(tr("Start:"));
    end = new QLabel(tr("End:"));

    lineSummary = new QLineEdit;
    lineSummary->setText(event.summary);
    lineLocation = new QLineEdit;
    lineLocation->setText(event.location);
    lineStart = new QTimeEdit;
    lineStart->setTime(QTime::fromString(event.dtstart,"HH:mm"));
    lineEnd = new QTimeEdit;
    lineEnd->setTime(QTime::fromString(event.dtend,"HH:mm"));

    connect(lineSummary, SIGNAL(textChanged(QString)), this, SLOT(enableEditRequestButton()));
    connect(lineLocation, SIGNAL(textChanged(QString)), this, SLOT(enableEditRequestButton()));

    summary->setBuddy(lineSummary);
    location->setBuddy(lineLocation);
    start->setBuddy(lineStart);
    start->setBuddy(lineEnd);

    //Create button
    createButton = new QPushButton("Save");
    createButton->setFixedWidth(70);
    createButton->setDefault(true);
    createButton->setEnabled(true);

    connect(createButton, SIGNAL(clicked()), this, SLOT(editEventRequest()));

    //Close button
    closeButton = new QPushButton(tr("Close"));
    closeButton->setFixedWidth(70);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QVBoxLayout *dayLayout = new QVBoxLayout;
    dayLayout->setAlignment(Qt::AlignHCenter);
    dayLayout->addWidget(day);

    QVBoxLayout *userTextLayout = new QVBoxLayout;
    userTextLayout->addWidget(summary);
    userTextLayout->addWidget(location);
    userTextLayout->addWidget(start);
    userTextLayout->addWidget(end);

    QVBoxLayout *insertTextLayout = new QVBoxLayout;
    insertTextLayout->addWidget(lineSummary);
    insertTextLayout->addWidget(lineLocation);
    insertTextLayout->addWidget(lineStart);
    insertTextLayout->addWidget(lineEnd);

    QHBoxLayout *smallBoxLayout = new QHBoxLayout;
    smallBoxLayout->addLayout(userTextLayout);
    smallBoxLayout->addLayout(insertTextLayout);

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->setAlignment(Qt::AlignHCenter);
    buttonLayout->addWidget(createButton);
    buttonLayout->addWidget(closeButton);

    QVBoxLayout *bigBoxLayout = new QVBoxLayout;

    bigBoxLayout->addLayout(dayLayout);
    bigBoxLayout->addLayout(smallBoxLayout);
    bigBoxLayout->addLayout(buttonLayout);
    setLayout(bigBoxLayout);

    setWindowTitle(tr("Create"));
    setFixedHeight(sizeHint().height());
}

void EditWindow::editEventRequest(){

    qDebug() << "editEventRequest\n";

    QUrl url("http://localhost/calendarserver.php/calendars/");
    url.setUserName(username);
    url.setPassword(password);

    QDateTime startDateTime;
    QDateTime endDateTime;

    startDateTime.setDate(QDate::fromString(event.date, "yyyyMMdd"));
    startDateTime.setTime(lineStart->time());

    endDateTime.setDate(QDate::fromString(event.date, "yyyyMMdd"));
    endDateTime.setTime(lineEnd->time());

    QString requestString = "BEGIN:VCALENDAR\r\n"
                            "BEGIN:VEVENT\r\n"
                            "UID:" + event.uid + "\r\n"
                            "VERSION:2.0\r\n"

                            "SUMMARY:" + lineSummary->text() + "\r\n"
                            "LOCATION:" + lineLocation->text() + "\r\n"
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

  request.setUrl(QUrl(url.toString() + username + "/default/" + event.uid + ".ics"));
  request.setRawHeader("Content-Type", "text/calendar; charset=utf-8");
  request.setRawHeader("Content-Length", contentlength);

  putReply = manager->put(request, buffer);

  if(NULL != putReply){
      //connect(putReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleError()));
      connect(putReply, SIGNAL(finished()), this, SLOT(editEventRequestFinished()));
  }
}

void EditWindow::editEventRequestFinished(){

    if(NULL != putReply){
        if(putReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 204) {
            qDebug() << "reply to edit executed";
            emit editFinished();
        }
        else {
            QMessageBox::information(this, "Error", "NETWORK ERROR!");
        }
        putReply->deleteLater();
    }
    this->close();
}

void EditWindow::enableEditRequestButton(){
createButton->setEnabled(!lineSummary->text().isEmpty() && !lineLocation->text().isEmpty()
                         && !lineStart->text().isEmpty() && !lineEnd->text().isEmpty());
}

