#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QJsonObject>
#include <QTimer>
#include <QDateTime>

class ServeurWebSocket : public QObject
{
    Q_OBJECT
public:
    explicit ServeurWebSocket(QObject *parent = nullptr);

public slots:
    void envoyerNouvelleInfoATous(QJsonObject obj);


private slots:
    void onQwebSocketServerNewConnection();
    void onQwebSocketDisconnected();


private:
    QWebSocketServer serveur;
    QList<QWebSocket *>listeClients;
};

#endif // WEBSOKCET_H
