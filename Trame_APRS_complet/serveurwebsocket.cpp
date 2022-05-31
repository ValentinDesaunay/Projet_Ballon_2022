#include "serveurwebsocket.h"
#include <QDebug>
#include <QJsonDocument>
#include <QRandomGenerator>

ServeurWebSocket::ServeurWebSocket(QObject *parent) : QObject(parent),serveur("monserveur",QWebSocketServer::NonSecureMode)
{
    if(serveur.listen(QHostAddress::Any,4545))
    {
        qDebug()<<"ok on ecoute sur le port 4545";
        connect(&serveur,&QWebSocketServer::newConnection,this,&ServeurWebSocket::onQwebSocketServerNewConnection);
    }
    else
    {
        qDebug()<<serveur.errorString();
    }
}

void ServeurWebSocket::onQwebSocketServerNewConnection()
{
    QWebSocket *client;
    client=serveur.nextPendingConnection();
    connect(client,&QWebSocket::disconnected,this,&ServeurWebSocket::onQwebSocketDisconnected);
    listeClients.append(client);
    qDebug()<<"connexion de "<<client->peerAddress().toString();
    qDebug()<<"nombre de clients actuel : "<<listeClients.count();
}

void ServeurWebSocket::onQwebSocketDisconnected()
{
    QWebSocket *client;
    client=qobject_cast<QWebSocket*>(sender()); //forcer le sender en QWebsocket*
    qDebug()<<"déconnexion de "<<client->peerAddress().toString();
    disconnect(client,&QWebSocket::disconnected,this,&ServeurWebSocket::onQwebSocketDisconnected);   //on désassocie l'événement connected
    listeClients.removeOne(client);
    client->deleteLater();  //supprimer le client en attendant que la boucle des événements soit passée
    qDebug()<<"nombre de clients actuel : "<<listeClients.count();

}

void ServeurWebSocket::envoyerNouvelleInfoATous(QJsonObject obj)
{

    QJsonDocument doc(obj);
    foreach(QWebSocket *client,listeClients)
    {
        client->sendTextMessage(doc.toJson(QJsonDocument::Compact).toStdString().c_str());     //convertion en chaine de caractère
        //standard et en texte simple avec c_str
    }

}
