#include "qtsingleapplication.h"
#include <QMessageBox>
#include "uimanager.h"

int main(int argc, char *argv[])
{
    QtSingleApplication a(argc, argv);

    if(a.isRunning()){
        QMessageBox::warning(nullptr,"Warning","app is running!","sure");
        return -1;
    }

    Uimanager manager;

    return a.exec();
}
