#ifndef EVENT_H
#define EVENT_H

#include <QString>

class Event
{
public:
    Event();
    Event(QString uid, QString summary, QString location, QString date, QString dtstart, QString dtend);
    QString uid;
    QString date;
    QString summary;
    QString location;
    QString dtstart;
    QString dtend;
};

#endif // EVENT_H
