#include <QApplication>
#include <QPushButton>
#include "Window.h"
#include "FindDialog.h"
#include "UserDialog.h"
#include <iostream>


int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    UserDialog *usrDialog = new UserDialog;
    usrDialog->show();





    //QPushButton button("Hello world!", nullptr);
    //button.resize(200, 100);
    //button.show();



    return QApplication::exec();
}
