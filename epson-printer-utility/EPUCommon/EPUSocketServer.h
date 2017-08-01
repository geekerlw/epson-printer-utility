#ifndef EPUSOCKETSERVER_H
#define EPUSOCKETSERVER_H

#include <QObject>
#include "EPUCommonDef.h"
#include "EPUNotificationCenter.h"
#include "EPUSocketClient.h"

class EPUSocketServer : public QObject, public EPUObserver
{
    Q_OBJECT
public:
    explicit EPUSocketServer(QObject *parent = 0);

public:
    void StartListen();
    
signals:
    
public slots:
    void didReceiveNewConnectionRequest();

private:
    QTcpServer*                     m_pTcpServer;
    QVector<EPUSocketClient*>       m_clientList;
    QMutex                          m_sockListMutex;

public:
    static void ClientDidDisconnect(EPUObserver* ptr, EPUObjectPtr object);
    void internalHandleClientDidDisconnect(EPUObjectPtr object);

    static void ClientDidConnect(EPUObserver* ptr, EPUObjectPtr object);
    void internalHandleClientDidConnect(EPUObjectPtr object);

    static void RequestStatus(EPUObserver* ptr, EPUObjectPtr object);
    void internalRequestStatus(EPUObjectPtr object);
};

#endif // EPUSOCKETSERVER_H
