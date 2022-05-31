<?php

require_once './fonctions.inc.php';
// test de la méthode d'envois des données
if (filter_input(INPUT_SERVER, 'REQUEST_METHOD') === 'GET') {
    // récupération de la donnée 'commande'
    $commande = filter_input(INPUT_GET, 'commande');
    switch ($commande) {
        case 'getListePositionsVehicule' :
            
            getListePositionsVehicule();
            
            break;
        case 'getListePositionsBallon' :
            
            getListePositionsBallon();
            
            break;
        default:
            header('Content-Type: application/json');
            echo json_encode("commande inconnue");
    }
}