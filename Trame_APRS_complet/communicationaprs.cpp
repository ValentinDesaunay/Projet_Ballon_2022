#include "communicationaprs.h"
#include <QDebug>
#include <QSettings>
#include <QFileInfo>
#include <QJsonDocument>
#include <QRandomGenerator>
#include <QThread>

CommunicationAPRS::CommunicationAPRS(QObject *parent): QObject(parent)
{
    socketDeDialogueAvecServeur=new QTcpSocket(this);
    connect(socketDeDialogueAvecServeur,&QTcpSocket::connected,this,&CommunicationAPRS::onQTcpSocket_connected);
    connect(socketDeDialogueAvecServeur,&QTcpSocket::disconnected,this,&CommunicationAPRS::onQTcpSocket_disconnected);
    connect(socketDeDialogueAvecServeur,&QTcpSocket::readyRead,this,&CommunicationAPRS::onQTcpSocket_readyRead);
    connect(socketDeDialogueAvecServeur,QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this,&CommunicationAPRS::onQTcpSocket_error);

    statut=chargerFichierIniConnexionAprs();

}

void CommunicationAPRS::connexionAPRS()
{
    socketDeDialogueAvecServeur->connectToHost(address,service_port);
    socketDeDialogueAvecServeur->write(loginMdp.toStdString().c_str(),loginMdp.length());
    socketDeDialogueAvecServeur->write(stationInfo.toStdString().c_str(),stationInfo.length());

}


void CommunicationAPRS::onQTcpSocket_connected()
{
    qDebug()<<"connecté";
}

void CommunicationAPRS::onQTcpSocket_disconnected()
{
    qDebug()<<"déconnecté";
}

void CommunicationAPRS::onQTcpSocket_readyRead()
{
    QByteArray donnees;
    qDebug()<<"reception data";
    donnees=socketDeDialogueAvecServeur->readAll();
    qDebug()<<donnees;
    emit donneesDisponible(donnees);
}


void CommunicationAPRS::onQTcpSocket_error(QAbstractSocket::SocketError socketError)
{
    qDebug()<<"vehicule"<<socketDeDialogueAvecServeur->errorString();
}

bool CommunicationAPRS::chargerFichierIniConnexionAprs()
{
    bool etat=false;

    QString nomFichierIni = "ConnexionAPRS.ini";
    QFileInfo fichierIni(nomFichierIni);
    if (fichierIni.exists() && fichierIni.isFile())
    {

        QSettings paramSocket(nomFichierIni,QSettings::IniFormat);
        CALLSIGN=paramSocket.value("SERVEURAPRS/CALLSIGN").toString();
        PASSCODE=paramSocket.value("SERVEURAPRS/PASSCODE").toString();
        LATITUDE=paramSocket.value("SERVEURAPRS/LATITUDE").toString();
        LONGITUDE=paramSocket.value("SERVEURAPRS/LONGITUDE").toString();
        PHG=paramSocket.value("SERVEURAPRS/PHG").toString();
        INFO=paramSocket.value("SERVEURAPRS/INFO").toString();
        DESTINATIONVEHICULE=paramSocket.value("SERVEURAPRS/DESTINATIONVEHICULE").toString();
        DESTINATIONBALLON=paramSocket.value("SERVEURAPRS/DESTINATIONBALLON").toString();
        callMobileVehicule=paramSocket.value("SERVEURAPRS/callMobileVehicule").toString();
        callMobileBallon=paramSocket.value("SERVEURAPRS/callMobileBallon").toString();
        address=paramSocket.value("SERVEURAPRS/address").toString();
        service_port=paramSocket.value("SERVEURAPRS/service_port").toInt();

        loginMdp = "user " + CALLSIGN + " pass " + PASSCODE + " vers " + INFO + " filter b/F4KMN*\n";
        stationInfo = CALLSIGN + ">" + DESTINATIONVEHICULE +","+DESTINATIONBALLON+ ":!"
                + LATITUDE + "I" + LONGITUDE + "&" + PHG + "/" + INFO+"\n";

        etat=true;
    }

    else
    {
        // si le fichier n’existe pas, il est créé avec les valeurs par défaut
        qDebug()<<"fichier ini non valide";
    }
    return etat;


}


QByteArray CommunicationAPRS::getDonnees() const
{
    return donnees;
}


