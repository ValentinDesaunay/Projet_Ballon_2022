#include <QCoreApplication>
#include "gestioncom.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    GestionCom gestCom;

    gestCom.lancementSuivi();


    return a.exec();
}
