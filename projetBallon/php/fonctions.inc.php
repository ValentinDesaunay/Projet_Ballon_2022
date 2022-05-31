<?php

define("SERVEURBD", "172.18.58.85");
define("LOGIN", "toto");
define("MOTDEPASSE", "toto");
define("NOMDELABASE", "BallonSonde2022");

/**
 * @brief crée la connexion avec la base de donnée et retourne l'objet PDO pour manipuler la base
 * @return \PDO
 */
//connexion à la base de données
function connexionBdd() {
    try {
        $pdoOptions = array(PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION);
        $bdd = new PDO('mysql:host=' . SERVEURBD . ';dbname=' . NOMDELABASE, LOGIN, MOTDEPASSE, $pdoOptions);
        $bdd->exec("set names utf8");
        return $bdd;
    } catch (PDOException $ex) {
        print "Erreur : " . $ex->getMessage() . "<br/>";
        die();
    }
}

//fonction pour récupérer la position du véhicule dans la base de données
function getListePositionsVehicule() {
    try {
        $bdd = connexionBdd();

        //requete sql
        $requete = $bdd->query("select latitude,longitude, horodatage from Vehicule;");
        
        //tableau de positions
        $tabPos = array();
        while ($ligne = $requete->fetch()) {
            array_push($tabPos, array(
                'longitude' => $ligne['longitude'],
                'latitude' => $ligne['latitude'],
                'horodatage' => $ligne['horodatage'],
                    )
            );
        }

        $requete->closeCursor();

        //on previent qu'on repond en json
        header('Content-Type: application/json;charset=utf-8');
        // envoyer les données au format json
        echo json_encode($tabPos);
    } catch (PDOException $ex) {
        print "Erreur : " . $ex->getMessage() . "<br/>";
        die();
    }
}

//fonction pour récupérer la position du ballon dans la base de données
function getListePositionsBallon() {
    try {
        $bdd = connexionBdd();

        //requete sql
        $requete = $bdd->query("select latitude,longitude, horodatage,altitude from TempsReel;");
        
        //tableau de positions
        $tabPos = array();
        while ($ligne = $requete->fetch()) {
            array_push($tabPos, array(
                'longitude' => $ligne['longitude'],
                'latitude' => $ligne['latitude'],
                'horodatage' => $ligne['horodatage'],
                'altitude' => $ligne['altitude'],
                    )
            );
        }

        $requete->closeCursor();

        //on previent qu'on repond en json
        header('Content-Type: application/json;charset=utf-8');
        // envoyer les données au format json
        echo json_encode($tabPos);
    } catch (PDOException $ex) {
        print "Erreur : " . $ex->getMessage() . "<br/>";
        die();
    }
}