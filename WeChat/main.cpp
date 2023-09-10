
#include "Login.h"
#include<Manager.h>
#include <QApplication>
#include<Globals.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Manager m;
    m.show ();
    return a.exec();
}
