#include "accesBDD.h"


AccesBDD::AccesBDD(QObject *parent) : QObject(parent)
{
    statut=chargerFichierIniAccesBdd();
}

bool AccesBDD::chargerFichierIniAccesBdd()
{
    bool etat=false;

    QString nomFichierIni = "ConnexionAPRS.ini";
    QFileInfo fichierIni(nomFichierIni);
    QSettings paramSocket(nomFichierIni,QSettings::IniFormat);
    if (fichierIni.exists() && fichierIni.isFile())
    {
        adresse=paramSocket.value("BDD/setHostName").toString();
        nomBase=paramSocket.value("BDD/setDataBaseName").toString();
        login=paramSocket.value("BDD/setUserName").toString();
        mdp=paramSocket.value("BDD/setPassword").toString();
        maxlat=paramSocket.value("VERIFVALEUR/valmaxlat").toString();
        minlat=paramSocket.value("VERIFVALEUR/valminlat").toString();
        maxlong=paramSocket.value("VERIFVALEUR/valmaxlong").toString();
        minlong=paramSocket.value("VERIFVALEUR/valminlong").toString();

        bd=QSqlDatabase::addDatabase("QMYSQL");
        bd.setHostName(adresse);
        bd.setDatabaseName(nomBase);
        bd.setUserName(login);
        bd.setPassword(mdp);

        if(!bd.open())
        {
            qDebug()<<"pb acces bd"<<bd.lastError();
        }
        else
        {
            qDebug()<<"acces bd ok";
            etat=true;
        }
    }else
    {
        qDebug()<<"le fichier ini n'existe pas";
    }
    return etat;

}

void AccesBDD::ajouterPositionVehicule(double latitude, double longitude, QDateTime horodatage)
{
    qDebug()<<"ajout position "<<horodatage;
    if(statut){ // les parametres du fichier ini ont ete chargees
        QSqlQuery requetePrepare;

        //appliquer une valeur min et max pour la latitude
        latitude=verifierValeur(latitude,minlat.toDouble(),maxlat.toDouble());
        //appliquer une valeur min et max pour la longitude
        longitude=verifierValeur(longitude,minlong.toDouble(),maxlong.toDouble());

        //requete pour mettre à jour la position dans la BDD
        requetePrepare.prepare("INSERT INTO Vehicule(latitude,longitude,horodatage)VALUES(:lat,:long,:horo)");
        requetePrepare.bindValue(":lat",latitude);
        requetePrepare.bindValue(":long",longitude);
        requetePrepare.bindValue(":horo",horodatage);

        if (!requetePrepare.exec()){
            qDebug()<<"pb requete "<<requetePrepare.lastError();
        }
    }

}

double AccesBDD::verifierValeur(double val, double valMin, double valMax)
{
    if(statut){
        //si la valeur rentrée est supérieure à la valeur max alors la valeur=valeurmax
        if(val>valMax) {
            val=valMax;
        }
        //si la valeur rentrée est inférieure à la valeur min alors la valeur=valeurmin
        if(val<valMin) {
            val=valMin;
        }
    }
    return val;
}


