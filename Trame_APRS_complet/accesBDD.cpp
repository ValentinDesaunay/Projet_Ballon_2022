/**
  @file accesBDD.cpp
  @brief Implémentation de la classe AccesBDD
  @version 1.0
  @author Valentin DESAUNAY
  @date 8/04/2022
  @details Classe modélisant l'accès à la BDD ainsi que la mise à jour du véhicule dans celle-ci
  */
#include "accesBDD.h"

/**
  @brief AccesBDD::AccesBDD
  @details Constructeur de la classe AccesBDD
  @param parent
  @author Valentin DESAUNAY
 */

AccesBDD::AccesBDD(QObject *parent) : QObject(parent)
{

    statut=chargerFichierIniAccesBdd();
}

/**
 * @brief AccesBDD::chargerFichierIniAccesBdd
 * @details Méthode qui charge le fichier ini pour se connecter à la BDD
 * @author Valentin DESAUNAY
 * @return bool
 */

bool AccesBDD::chargerFichierIniAccesBdd()
{
    //etat est initialisé à false
    bool etat=false;

    //déclaration du fichier ini
    QString nomFichierIni = "ConnexionAPRS.ini";
    QFileInfo fichierIni(nomFichierIni);
    QSettings paramSocket(nomFichierIni,QSettings::IniFormat);
    //on vérifie que le fichier ini existe et est un fichier
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

        //connexion à la base de données
        bd=QSqlDatabase::addDatabase("QMYSQL");
        bd.setHostName(adresse);
        bd.setDatabaseName(nomBase);
        bd.setUserName(login);
        bd.setPassword(mdp);

        if(!bd.open())
        {
            //si la connexion à la BDD ne s'effectue pas
            qDebug()<<"pb acces bd"<<bd.lastError();
        }
        else
        {
            //si la connexion à la BDD se fait
            qDebug()<<"acces bd ok";
            etat=true;
        }
    }else
    {
        //si le fichier INI n'existe pas
        qDebug()<<"le fichier ini n'existe pas";
    }
    //retourne si etat est true ou false
    return etat;

}
/**
 * @brief AccesBDD::ajouterPositionVehicule
 * @param latitude du véhicule
 * @param longitude du véhicule
 * @param horodatage de la position
 * @author Valentin DESAUNAY
 */

void AccesBDD::ajouterPositionVehicule(double latitude, double longitude, QDateTime horodatage)
{
    qDebug()<<"ajout position "<<horodatage;
    if(statut){ // les parametres du fichier ini ont été chargés
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

        //si la requête ne s'execute pas
        if (!requetePrepare.exec()){
            qDebug()<<"pb requete "<<requetePrepare.lastError();
        }
    }

}
/**
 * @brief AccesBDD::verifierValeur
 * @param val valeur de la latitude ou longitude du véhicule
 * @param valMin valeur minimale de la latitude ou longitude du véhicule
 * @param valMax valeur maximale de la latitude ou longitude du véhicule
 * @return val
 * @author Valentin DESAUNAY
 */

double AccesBDD::verifierValeur(double val, double valMin, double valMax)
{
    //si les paramètres du fichier ini sont chargés
    if(statut){
        //si la valeur est supérieure à la valeur max alors la valeur=valeurmax
        if(val>valMax) {
            val=valMax;
        }
        //si la valeur est inférieure à la valeur min alors la valeur=valeurmin
        if(val<valMin) {
            val=valMin;
        }
    }
    //retourne la valeur
    return val;
}


