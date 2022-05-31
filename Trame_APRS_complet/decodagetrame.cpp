#include "decodagetrame.h"
#include <QFileInfo>
#include <QSettings>

using namespace std;

DecodageTrame::DecodageTrame(QObject *parent): QObject(parent)
{
    statut=chargerFichierIni();
}


bool DecodageTrame::chargerFichierIni()
{
    bool etat=false;

    QString nomFichierIni = "ConnexionAPRS.ini";
    QFileInfo fichierIni(nomFichierIni);
    if (fichierIni.exists() && fichierIni.isFile())
    {
        QSettings paramSocket(nomFichierIni,QSettings::IniFormat);
        DESTINATIONVehicule=paramSocket.value("SERVEURAPRS/DESTINATIONVEHICULE").toString();
        qDebug()<<"id vehi : "<<DESTINATIONVehicule;
        DESTINATIONBallon=paramSocket.value("SERVEURAPRS/DESTINATIONBALLON").toString();
        qDebug()<<"id ballon : "<<DESTINATIONBallon;
    }
    else
    {
        qDebug()<<"fichier ini non valide";
    }
    return etat;

}

void DecodageTrame::decoderTrameVehicule(QByteArray donnees)
{

    QString ID=donnees.mid(0,8);

    //  qDebug()<<donnees;
    if(ID==DESTINATIONVehicule)
    {
        qDebug()<<"donnees vehicule";
        qDebug()<<donnees;
        int positionDebut=1+donnees.indexOf("=");
        QString Position=donnees.mid(positionDebut,18);
        qDebug()<<Position;
        QString latitudeChaine=Position.mid(0,8);
        qDebug()<<"latitude : " + latitudeChaine;
        QString longitudeChaine=Position.mid(9,9);
        qDebug()<<"longitude : " + longitudeChaine;

        QString orientationNS=latitudeChaine.mid(7,1);
        double minutesLatitude=latitudeChaine.mid(2,5).toDouble();
        double degresLatitude=latitudeChaine.mid(0,2).toDouble();


        qDebug()<<"Orientation latitude : "+orientationNS;
        qDebug()<<"Degré latitude : "+QString::number(degresLatitude);
        qDebug()<<"Minutes latitude: "+QString::number(minutesLatitude);


        QString orientationOE=longitudeChaine.mid(8,1);
        double minutesLongitude=longitudeChaine.mid(3,5).toDouble();
        double degresLongitude=longitudeChaine.mid(0,3).toDouble();


        qDebug()<<"Orientation longitude : "+orientationOE;
        qDebug()<<"Degré longitude: "+QString::number(degresLongitude);
        qDebug()<<"Minutes longitude : "+QString::number(minutesLongitude);

        //Conversion de Degré Minutes Décimale en Degré décimale

        latitude=degresLatitude+(minutesLatitude/60.0);
        longitude=degresLongitude+(minutesLongitude/60.0);


        if(orientationNS=="S")
        {
            latitude=-latitude;
        }
        if(orientationOE=="W")
        {

            longitude=-longitude;
        }

        qDebug()<<"La latitude en degré décimale : " +QString::number(latitude);
        qDebug()<<"La longitude en degré décimale : " +QString::number(longitude);
        horodatage=QDateTime::currentDateTime();
        qDebug()<<horodatage;
        emit donneesDecodees(latitude,longitude,horodatage);

        QJsonObject obj;
        obj.insert("latitude",QJsonValue(QString::number(latitude)));
        obj.insert("longitude",QJsonValue(QString::number(longitude)));
        qDebug()<<horodatage.toString();
        obj.insert("horodatage",QJsonValue(horodatage.toString("dddd dd MMMM yyyy hh:mm:ss")));
        obj.insert("emetteur",QJsonValue("vehicule"));
        obj.insert("source",QJsonValue("APRS"));
        obj.insert("id",QJsonValue(ID));
        emit nouvellesDonneesVehicule(obj);

    }

}
/**
 * @brief DecodageTrame::decoderTrameBallon
 * @details
 * @param donnees
 */
void DecodageTrame::decoderTrameBallon(QByteArray donnees)
{
    QString ID=donnees.mid(0,8);

    if(ID==DESTINATIONBallon)
    {
        qDebug()<<"donnees ballon";
        qDebug()<<donnees;
        int positionDebut=1+donnees.indexOf("!");
        QString Position=donnees.mid(positionDebut,28);
        qDebug()<<Position;
        QString latitudeChaine=Position.mid(0,8);
        qDebug()<<"latitude : " + latitudeChaine;
        QString longitudeChaine=Position.mid(9,9);
        qDebug()<<"longitude : " + longitudeChaine;
        QString altitudeChaine=Position.mid(22,6);
        qDebug()<<"altitude : "+altitudeChaine;

        QString orientationNS=latitudeChaine.mid(7,1);
        double minutesLatitude=latitudeChaine.mid(2,5).toDouble();
        double degresLatitude=latitudeChaine.mid(0,2).toDouble();



        qDebug()<<"Orientation latitude : "+orientationNS;
        qDebug()<<"Degré latitude : "+QString::number(degresLatitude);
        qDebug()<<"Minutes latitude: "+QString::number(minutesLatitude);


        QString orientationOE=longitudeChaine.mid(8,1);
        double minutesLongitude=longitudeChaine.mid(3,5).toDouble();
        double degresLongitude=longitudeChaine.mid(0,3).toDouble();

        qDebug()<<"Orientation longitude : "+orientationOE;
        qDebug()<<"Degré longitude: "+QString::number(degresLongitude);
        qDebug()<<"Minutes longitude : "+QString::number(minutesLongitude);

        //Conversion de Degré Minutes Décimale en Degré décimale

        latitude=degresLatitude+(minutesLatitude/60.0);
        longitude=degresLongitude+(minutesLongitude/60.0);
        altitudeBallon=altitudeChaine.toDouble()*0.3048;

        if(orientationNS=="S")
        {
            latitude=-latitude;
        }
        if(orientationOE=="W")
        {

            longitude=-longitude;
        }

        qDebug()<<"La latitude en degré décimale : " +QString::number(latitude);
        qDebug()<<"La longitude en degré décimale : " +QString::number(longitude);
        horodatage=QDateTime::currentDateTime();
        qDebug()<<horodatage;
        qDebug()<<altitudeBallon;

        QJsonObject obj;
        obj.insert("latitude",QJsonValue(QString::number(latitude)));
        obj.insert("longitude",QJsonValue(QString::number(longitude)));
        obj.insert("altitudeBallon",QJsonValue(QString::number(altitudeBallon)));
        obj.insert("horodatage",QJsonValue(horodatage.toString("dddd dd MMMM yyyy hh:mm:ss")));
        obj.insert("emetteur",QJsonValue("ballon"));
        obj.insert("source",QJsonValue("APRS"));
        obj.insert("id",QJsonValue(ID));
        emit nouvellesDonneesBallon(obj);

    }
}

double DecodageTrame::getLongitude() const
{
    return longitude;
}

double DecodageTrame::getLatitude() const
{
    return latitude;
}

