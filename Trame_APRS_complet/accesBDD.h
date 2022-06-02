/**
  @file accesBDD.h
  @brief Déclaration de la classe AccesBDD
  @version 1.0
  @author Valentin DESAUNAY
  @date 8/04/2022
  */
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

/** La classe AccesBDD hérite de QObjcect*/

class AccesBDD : public QObject
{
    Q_OBJECT
public:
    explicit AccesBDD(QObject *parent = nullptr);
public slots:
    void ajouterPositionVehicule(double latitude,double longitude,QDateTime horodatage);
private:
    /** base de données*/
    QSqlDatabase bd;
    /** Adresse IP de la base de données */
    QString adresse;
    /** Le login de la base de données */
    QString login;
    /** Le mot de passe de la base de données */
    QString mdp;
    /** Le nom de la base de données */
    QString nomBase;
    /** La longitude du véhicule*/
    double longitude;
    /** La latitude du véhicule*/
    double latitude;
    /** Statut du fichier ini*/
    bool statut;
    /** Valeur max de la latitude*/
    QString maxlat;
    /** Valeur min de la latitude*/
    QString minlat;
    /** Valeur max de la longitude*/
    QString maxlong;
    /** Valeur min de la longitude*/
    QString minlong;


    double verifierValeur(double val,double valMin,double valMax);
    bool chargerFichierIniAccesBdd();

signals:
    void nouvellesDonneesBallon(QJsonObject obj);

};

#endif // MISEAJOURPOSITIONBDD_H
