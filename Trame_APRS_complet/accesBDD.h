#ifndef MISEAJOURPOSITIONBDD_H
#define MISEAJOURPOSITIONBDD_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QFileInfo>
#include <QSettings>

#include "communicationaprs.h"
#include "decodagetrame.h"

class AccesBDD : public QObject
{
    Q_OBJECT
public:
    explicit AccesBDD(QObject *parent = nullptr);
    QSqlDatabase bd;
public slots:
    void ajouterPositionVehicule(double latitude,double longitude,QDateTime horodatage);
private:
    QString adresse;
    QString login;
    QString mdp;
    QString nomBase;
    double longitude;
    double latitude;
    int altitude;
    bool statut;
    QString maxlat;
    QString minlat;
    QString maxlong;
    QString minlong;

    double verifierValeur(double val,double valMin,double valMax);
    bool chargerFichierIniAccesBdd();

signals:
    void nouvellesDonneesBallon(QJsonObject obj);

};

#endif // MISEAJOURPOSITIONBDD_H
