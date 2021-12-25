#include <QApplication>
#include "userdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UserDialog* usrDialog = new UserDialog();
    usrDialog->show();

    return a.exec();
}
