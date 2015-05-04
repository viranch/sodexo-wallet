#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("CnV Inc.");
    QCoreApplication::setApplicationName("Sodexo");
    Dialog w;
    w.show();

    return a.exec();
}
