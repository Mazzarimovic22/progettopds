#include "editwindow.h"

EditWindow::EditWindow(QString username, QString password, QDate date, QNetworkAccessManager* manager, QWidget *parent):
    QWidget(parent), username(username), password(password), date(date), manager(manager){

    day = new QLabel();
    day->setText(date.toString("dd/MM/yyyy"));
    day->setStyleSheet("font-size: 20px;");

    summary = new QLabel(tr("Summary:"));
    location = new QLabel(tr("Location:"));
    start = new QLabel(tr("Start:"));
    end = new QLabel(tr("End:"));

    lineSummary = new QLineEdit;
    lineLocation = new QLineEdit;
    lineStart = new QTimeEdit;
    lineEnd = new QTimeEdit;

    connect(lineSummary, SIGNAL(textChanged(QString)), this, SLOT(enableEditRequestButton()));
    connect(lineLocation, SIGNAL(textChanged(QString)), this, SLOT(enableEditRequestButton()));

    summary->setBuddy(lineSummary);
    location->setBuddy(lineLocation);
    start->setBuddy(lineStart);
    start->setBuddy(lineEnd);

    //Create button
    createButton = new QPushButton(tr("&Add"));
    createButton->setFixedWidth(70);
    //loginButton->setStyleSheet("font: bold;background-color: red;font-size: 36px;height: 48px;width: 2px;");
    createButton->setDefault(true);
    createButton->setEnabled(false);
    connect(createButton, SIGNAL(clicked()), this, SLOT(editRequest()));

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


void EditWindow::editRequest(){

//TO-DO: CONTROLLARE CHE START SIA < DI END

QUrl url("http://localhost/calendarserver.php/calendars/");
url.setUserName(username);
url.setPassword(password);

QString uid = QDateTime::currentDateTime().toString("yyyyMMdd-HHMM-00ss") + "-0000-" + lineSummary->text();

QDateTime startDateTime;
QDateTime endDateTime;

startDateTime.setDate(date);
startDateTime.setTime(lineStart->time());

endDateTime.setDate(date);
endDateTime.setTime(lineEnd->time());

QString requestString = "BEGIN:VCALENDAR\r\n"
                        "BEGIN:VEVENT\r\n"
                        "UID:" + uid + "\r\n"
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

  request.setUrl(QUrl(url.toString() + username + "/default/" + uid + ".ics"));
  request.setRawHeader("Content-Type", "text/calendar; charset=utf-8");
  request.setRawHeader("Content-Length", contentlength);

  manager->put(request, buffer);

  connect(manager,&QNetworkAccessManager::finished, [this](QNetworkReply *reply){

    if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 201) {
        this->close();
    }
    else {
        QMessageBox::information(this, "Error", "NETWORK ERROR!");
    }
    reply->deleteLater();
  });
}

void EditWindow::enableEditRequestButton(){
createButton->setEnabled(!lineSummary->text().isEmpty() && !lineLocation->text().isEmpty()
                         && !lineStart->text().isEmpty() && !lineEnd->text().isEmpty());
}

