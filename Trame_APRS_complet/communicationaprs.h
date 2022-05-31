#ifndef COMMUNICATIONAPRS_H
#define COMMUNICATIONAPRS_H
#include <QTcpSocket>
#include <QString>
#include <QJsonObject>
#include <QTimer>
#include <QWebSocketServer>
#include <QWebSocket>

#include "decodagetrame.h"



class CommunicationAPRS :  public QObject

{
    Q_OBJECT
public:
    CommunicationAPRS(QObject *parent = nullptr);

    QByteArray getDonnees() const;
    void connexionAPRS();

private slots:

    void onQTcpSocket_connected();

    void onQTcpSocket_disconnected();

    void onQTcpSocket_readyRead();

    void onQTcpSocket_error(QAbstractSocket::SocketError socketError);


signals:
    void donneesDisponible(QByteArray);
private:
    QTcpSocket *socketDeDialogueAvecServeur;
    QByteArray donnees;
    QString CALLSIGN;
    QString PASSCODE;
    QString LATITUDE;
    QString LONGITUDE;
    QString PHG;
    QString INFO;
    QString DESTINATIONVEHICULE;
    QString DESTINATIONBALLON;
    QString callMobileVehicule;
    QString callMobileBallon;
    QString address;
    int service_port;
    QString loginMdp;
    QString stationInfo;
    bool statut;
    bool chargerFichierIniConnexionAprs();
    //QString loginMdpVehicule;
    //QString stationInfoVehicule;


};

#endif // COMMUNICATIONAPRS_H

