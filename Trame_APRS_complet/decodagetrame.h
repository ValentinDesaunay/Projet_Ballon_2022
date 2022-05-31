#ifndef DECODAGETRAME_H
#define DECODAGETRAME_H
#include <QTcpSocket>
#include <QDateTime>
#include <QJsonObject>


class DecodageTrame :  public QObject
{
    Q_OBJECT
public:
    DecodageTrame(QObject *parent=nullptr);

    double getLatitude() const;

    double getLongitude() const;
signals:
    void donneesDecodees(double latitude,double longitude,QDateTime horodatage);
    void nouvellesDonneesVehicule(QJsonObject obj);
    void nouvellesDonneesBallon(QJsonObject obj);

public slots:
    void decoderTrameVehicule(QByteArray donnees);
    void decoderTrameBallon(QByteArray donnees);
private:
    double latitude;
    double longitude;
    double altitudeBallon;
    QDateTime horodatage;
    QString DESTINATIONVehicule;
    QString DESTINATIONBallon;
    bool chargerFichierIni();
    bool statut;


};

#endif // DECODAGETRAME_H
