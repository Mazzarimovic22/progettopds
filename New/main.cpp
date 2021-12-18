#include <QApplication>
#include "userdialog.h"
#include "todoapps.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UserDialog* usrDialog = new UserDialog();
    usrDialog->show();

    //ToDoApps* tDo = new ToDoApps();
    return a.exec();
}
