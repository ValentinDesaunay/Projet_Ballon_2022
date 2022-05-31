var lat = 48.852969;
var lon = 2.349903;
var maCarte = null;
var marqueurDyna;
var marqueurBallon = null;
var marqueurVoiture = null;
var cercle;
var polygone;
var groupe = new L.featureGroup();
var maWebsocketVehicule;
var maWebsocketBallon;

var coordonnee;
var url;

//chemin des images pour les différents marqueurs
const VOITURE = "img/Marqueur_voiture.png"
const BALLON = "img/Marqueur_ballon.png"

//création de l'icône pour la voiture
var myIconVoiture = L.icon({
    iconUrl: VOITURE,
    iconRetinaUrl: VOITURE,
    iconSize: [65, 65],
    iconAnchor: [33, 62],
    popupAnchor: [0, -70],

});

//création de l'icône pour le ballon
var myIconBallon = L.icon({
    iconUrl: BALLON,
    iconRetinaUrl: BALLON,
    iconSize: [65, 65],
    iconAnchor: [33, 62],
    popupAnchor: [0, -70],

});

var popupVoiture;
var popupBallon;

function initCarte() {
    maCarte = L.map('carte').setView([lat, lon], 7);
    // Leaflet ne récupère pas les cartes (tiles) sur un serveur par défaut. Nous devons lui préciser où 
    // nous souhaitons les récupérer. Ici, openstreetmap.fr
    L.tileLayer('https://{s}.tile.openstreetmap.fr/osmfr/{z}/{x}/{y}.png', {
        //lien vers la source des données
        attribution: 'données © <a href="//osm.org/copyright">OpenStreetMap</a>/ODbL\n\
 - rendu <a href="//openstreetmap.fr">OSM France</a>',
        minZoom: 6,
        maxZoom: 20
    }).addTo(maCarte);
    /*
     marqueurBallon = L.marker([47.995311, 0.204986], {icon: myIconBallon}).addTo(maCarte);
     marqueurBallon.bindPopup("Point départ ballon");
     groupe.addLayer(marqueurBallon);
     maCarte.fitBounds(groupe.getBounds());
     */
    $.getJSON('php/controleur.php',
            {
                'commande': 'getListePositionsVehicule',
            }
    )
            .done(function (donnees, stat, xhr) {
                console.log("Taille tableau données vehicule : " + donnees.length);
                var tailleTab = donnees.length;


                $.each(donnees, function (index, ligne) {
                    //console.log("index : " + index);
                    //transformer une chaîne de caractères en un nombre flottant
                    var lati = parseFloat(ligne.latitude);
                    var longi = parseFloat(ligne.longitude);
                    // arrondir latitude et longitude à 6 chiffres après la virgule
                    lati = lati.toFixed(6);
                    longi = longi.toFixed(6)
                    //si cest le dernier indice du tableau alors cest la dernière position
                    //sinon ce n'est pas la dernière
                    if (index == tailleTab - 1) // dernier enregistrement
                    {
                        initMarqueurVoiture(lati, longi, ligne.horodatage, true);
                        console.log("Dernière position");
                    }
                    /*else
                     { //Affiche un cercle pour toutes les positions sauf la dernière de la BDD
                     initMarqueurVoiture(lati, longi, ligne.horodatage, false);
                     console.log("Pas la dernière");
                     }*/
                });
                //ligne pour les positions présentes dans la BDD
                /*
                 console.log(donnees);
                 for (var i = 0; i < tailleTab - 1; i++) {
                 var latLng1 = new L.LatLng(donnees[i].latitude, donnees[i].longitude);
                 var newlatLng = new L.LatLng(donnees[i + 1].latitude, donnees[i + 1].longitude);
                 var polyline = L.polyline([latLng1, newlatLng], {color: 'red', weight: 4}).addTo(maCarte);
                 }
                 */
            })
            .fail(function (xhr, text, error) {
                console.log("param : " + JSON.stringify(xhr));
                console.log("status : " + text);
                console.log("error : " + error);
            });


    $.getJSON('php/controleur.php',
            {
                'commande': 'getListePositionsBallon',
            }
    )
            .done(function (donnees, stat, xhr) {
                console.log("Taille tableau données ballon : " + donnees.length);
                var tailleTab = donnees.length;

                coordonnee = donnees;
                $.each(donnees, function (index, ligne) {
                    //console.log("index : " + index);
                    //transformer une chaîne de caractères en un nombre flottant
                    var lat = parseFloat(ligne.latitude);
                    var longi = parseFloat(ligne.longitude);
                    // arrondir latitude et longitude à 6 chiffres après la virgule
                    lat = lat.toFixed(6);
                    longi = longi.toFixed(6)
                    //si cest le dernier indice du tableau alors cest la dernière position
                    //sinon ce n'est pas la dernière
                    if (index == tailleTab - 1) // dernier enregistrement
                    {
                        marqueurBallon = L.marker([lat, longi], {icon: myIconBallon}).addTo(maCarte);
                        popupBallon = lat + " / " + longi + "<br/>" + ligne.altitude + "m / " + ligne.horodatage;
                        marqueurBallon.bindPopup(popupBallon);
                        groupe.addLayer(marqueurBallon);
                        maCarte.fitBounds(groupe.getBounds());
                    }
                });
            })
            .fail(function (xhr, text, error) {
                console.log("param : " + JSON.stringify(xhr));
                console.log("status : " + text);
                console.log("error : " + error);
            });
}


function initMarqueurVoiture(latitude, longitude, horodatage, dernierePosition)
{
    //transformer une chaîne de caractères en un nombre flottant
    var lat = parseFloat(latitude);
    var longi = parseFloat(longitude);
    // arrondir latitude et longitude à 6 chiffres après la virgule
    lat = lat.toFixed(6);
    longi = longi.toFixed(6);
    // si cest la dernière position alors cela affiche un marqueur voiture sinon afffiche marqueur bleu
    if (dernierePosition)
    {
        //création du marqueur
        marqueurVoiture = L.marker([latitude, longitude], {icon: myIconVoiture}).addTo(maCarte);
        //afficher un popup avec latitude, longitude et horodatage de la position
        popupVoiture = lat + " / " + longi + "<br/>" + horodatage;
        marqueurVoiture.bindPopup(popupVoiture);
        //ajouter mon marqueur à mon groupe d'entités
        groupe.addLayer(marqueurVoiture);
        maCarte.fitBounds(groupe.getBounds());

    } else {
        marqueurDyna = L.circle([latitude, longitude], {radius: 0.6, color: "red", fillOpacity: 1}).addTo(maCarte);
        //afficher un popup avec latitude, longitude et horodatage de la position
        marqueurDyna.bindPopup(lat + " / " + longi + "<br/>" + horodatage);
        //ajouter mon marqueur à mon groupe d'entités
        //groupe.addLayer(marqueurDyna);
        //maCarte.fitBounds(groupe.getBounds());
    }

}

function ajouterMarqueurBallon(latitude, longitude, altitude, horodatage, source)
{

    //transformer une chaîne de caractères en un nombre flottant
    var lat = parseFloat(latitude);
    var longi = parseFloat(longitude);
    var alti = parseFloat(altitude);
    // arrondir latitude et longitude à 6 chiffres après la virgule
    lat = lat.toFixed(6);
    longi = longi.toFixed(6);
    alti = alti.toFixed(1);

    if (marqueurBallon === null) {

        marqueurBallon = L.marker([lat, longi], {icon: myIconBallon}).addTo(maCarte);
        marqueurBallon.bindPopup(lat + " / " + longi + "<br/>" + alti + "m / " + horodatage);
        groupe.addLayer(marqueurBallon);
        maCarte.fitBounds(groupe.getBounds());

    } else {

        // get ancienne position
        var lastLatLng = marqueurBallon.getLatLng();
        console.log("ancienne position ballon " + lastLatLng);
        // set nouvelle coordonne
        var newlatLng = new L.LatLng(latitude, longitude);
        // ajout point ancienne coordonnee
        marqueurDyna = L.circle(lastLatLng , {radius: 0.6, color: "blue", fillOpacity: 1}).addTo(maCarte);
        marqueurDyna.bindPopup(popupBallon);   
        // ajout ligne
        var polyline = L.polyline([newlatLng, lastLatLng], {color: 'blue', weight: 4}).addTo(maCarte);
        // deplacer marqueur
        marqueurBallon.setLatLng(newlatLng);
        
        if (source === "APRS")
        {
            popupBallon = "APRS" + " / " + lat + " / " + longi + "<br/>" + alti + "m / " + horodatage;
            marqueurBallon.bindPopup(popupBallon);
            groupe.addLayer(marqueurBallon);
            maCarte.fitBounds(groupe.getBounds());
        } else
        {
            popupBallon = "SIGFOX" + " / " + lat + " / " + longi + "<br/>" + alti + "m / " + horodatage;
            marqueurBallon.bindPopup(popupBallon);
            groupe.addLayer(marqueurBallon);
            maCarte.fitBounds(groupe.getBounds());
        }
    }

}

function ajouterMarqueurVoiture(latitude, longitude, horodatage)
{

    //transformer une chaîne de caractères en un nombre flottant
    var lat = parseFloat(latitude);
    var longi = parseFloat(longitude);
    // arrondir latitude et longitude à 6 chiffres après la virgule
    lat = lat.toFixed(6);
    longi = longi.toFixed(6);

    if (marqueurVoiture === null) {

        marqueurVoiture = L.marker([lat, longi], {icon: myIconVoiture}).addTo(maCarte);
        popupVoiture = lat + " / " + longi + "<br/>" + horodatage;
        marqueurVoiture.bindPopup(popupVoiture);
        groupe.addLayer(marqueurVoiture);
        maCarte.fitBounds(groupe.getBounds());

    } else {

        // get ancienne position
        var lastLatLng = marqueurVoiture.getLatLng();
        console.log("ancienne position voiture " + lastLatLng.toString());
        // set nouvelle coordonne
        var newlatLng = new L.LatLng(lat, longi);
        // ajout point ancienne coordonnee
        marqueurDyna = L.circle(lastLatLng, {radius: 0.6, color: "red", fillOpacity: 1}).addTo(maCarte);
        marqueurDyna.bindPopup(popupVoiture);
        // ajout ligne
        var polyline = L.polyline([newlatLng, lastLatLng], {color: 'red', weight: 4}).addTo(maCarte);
        // deplacer marqueur
        marqueurVoiture.setLatLng(newlatLng);
        popupVoiture = "APRS" + " / " + "" + lat + " / " + longi + "<br/>" + horodatage;
        marqueurVoiture.bindPopup(popupVoiture);
        groupe.addLayer(marqueurVoiture);
        maCarte.fitBounds(groupe.getBounds());
    }
}

//fonction pour se connecter au serveur
function connexionServeurWebSocketVehicule()
{
    url = new URL(location.href);

    if (window.WebSocket)
    {
        // creation de la WebSocket
        maWebsocketVehicule = new WebSocket('ws://' + url.hostname + ':4545');

        //Gestion des evenements de la WebSocket
        maWebsocketVehicule.onopen = function ()
        {
            console.log("ouverture webSocket Vehicule");
        };
        maWebsocketVehicule.onclose = function (event)
        {
            console.log("code de la fermeture : " + event.code);
            console.log("raison de la fermeture : " + event.reason);
        };
        maWebsocketVehicule.onerror = function ()
        {
            console.log("erreur sur la webSocket Vehicule");
        };
        maWebsocketVehicule.onmessage = function (donneesRecues)
        {
            console.log("reception de donnees : " + donneesRecues.data);
            console.log("origine : " + donneesRecues.origin);
            var obj = JSON.parse(donneesRecues.data);
            //afficher latitude,longitude et horodatage
            console.log("données de l'APRS");
            console.log("emetteur :" + obj.emetteur);
            console.log("source :" + obj.source);
            console.log("latitude : " + obj.latitude);
            console.log("longitude : " + obj.longitude);
            console.log("altitudeBallon : " + obj.altitudeBallon);
            console.log("horodatage : " + obj.horodatage);
            if (obj.emetteur == "vehicule") {
                ajouterMarqueurVoiture(obj.latitude, obj.longitude, obj.horodatage);

            } else
            {
                ajouterMarqueurBallon(obj.latitude, obj.longitude, obj.altitudeBallon, obj.horodatage, obj.source);

            }
        };
    }
}

function connexionServeurWebSocketBallonSigfox()
{
    url = new URL(location.href);
    if (window.WebSocket)
    {
        // creation de la WebSocket
        // maWebsocketBallon = new WebSocket('ws://195.221.61.190:4544');
        maWebsocketBallon = new WebSocket('ws://' + url.hostname + ':4544');

        //Gestion des evenements de la WebSocket
        maWebsocketBallon.onopen = function ()
        {
            console.log("ouverture webSocket Ballon");
        };
        maWebsocketBallon.onclose = function (event)
        {
            console.log("code de a la fermeture : " + event.code);
            console.log("raison de la fermeture : " + event.reason);
        };
        maWebsocketBallon.onerror = function ()
        {
            console.log("erreur sur la webSocket");
        };
        maWebsocketBallon.onmessage = function (donneesRecues)
        {
            console.log("reception de donnees : " + donneesRecues.data);
            console.log("origine : " + donneesRecues.origin);
            var obj = JSON.parse(donneesRecues.data);
            var date = new Date(obj.horodatage);
            var tabJour = new Array("Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi");
            var tabMois = new Array("Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin", "Juillet", "Aout", "Septembre", "Octobre", "Novembre", "Decembre");
            obj.horodatage = tabJour[date.getDay()] + " " + date.getDate() + " " + tabMois[date.getMonth()] + " " + date.getFullYear() + " " + date.getHours() + ":" + date.getUTCMinutes() + ":" + date.getSeconds();
            //afficher latitude,longitude et horodatage
            console.log("données serveur ballon");
            console.log("source : " + obj.source);
            console.log("latitude : " + obj.latitude);
            console.log("longitude : " + obj.longitude);
            console.log("horodatage : " + obj.horodatage);
            //afficher l'altitude du ballon
            console.log("altitude : " + obj.altitude);
            ajouterMarqueurBallon(obj.latitude, obj.longitude, obj.altitude, obj.horodatage, obj.source);
        };
    }
}

// affichage de la carte au chargement de la page
// via l'appel à la fonction initCarte
//$(window).on('load', initCarte);
// affichage de la connexion au serveur au chargement de la page
// via l'appel à la fonction initCarte
//$(window).on('load', connexionServeurWebSocketVehicule());
//$(window).on('load', connexionServeurWebSocketBallonSigfox());
$(document).ready(function () {

    initCarte();
    connexionServeurWebSocketVehicule();
    connexionServeurWebSocketBallonSigfox();
});