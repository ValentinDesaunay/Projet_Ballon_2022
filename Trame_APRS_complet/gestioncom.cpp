#include "gestioncom.h"

GestionCom::GestionCom(QObject *parent) : QObject(parent)
{
    QObject::connect(&aprs,&CommunicationAPRS::donneesDisponible,&decodeur,&DecodageTrame::decoderTrameVehicule);
    QObject::connect(&aprs,&CommunicationAPRS::donneesDisponible,&decodeur,&DecodageTrame::decoderTrameBallon);
    QObject::connect(&decodeur,&DecodageTrame::nouvellesDonneesVehicule,&serveur,&ServeurWebSocket::envoyerNouvelleInfoATous);
    QObject::connect(&decodeur,&DecodageTrame::nouvellesDonneesBallon,&serveur,&ServeurWebSocket::envoyerNouvelleInfoATous);
    QObject::connect(&decodeur,&DecodageTrame::donneesDecodees,&bd,&AccesBDD::ajouterPositionVehicule);

}

GestionCom::~GestionCom()
{

}


void GestionCom::lancementSuivi()
{
    aprs.connexionAPRS();
}
