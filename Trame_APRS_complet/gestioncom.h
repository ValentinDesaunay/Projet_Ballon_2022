#ifndef GESTIONCOM_H
#define GESTIONCOM_H
#include "serveurwebsocket.h"
#include "communicationaprs.h"
#include "decodagetrame.h"
#include "accesBDD.h"

#include <QObject>

class GestionCom : public QObject
{
    Q_OBJECT
public:
    explicit GestionCom(QObject *parent = nullptr);
    ~GestionCom();
    void lancementSuivi();
private:
    CommunicationAPRS aprs;
    ServeurWebSocket serveur;
    DecodageTrame decodeur;
    AccesBDD bd;

};

#endif // GESTIONCOM_H
