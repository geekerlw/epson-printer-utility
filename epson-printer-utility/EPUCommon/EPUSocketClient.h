#ifndef EPUSOCKETCLIENT_H
#define EPUSOCKETCLIENT_H

#include <QObject>
#include "EPUCommonDef.h"
#include "EPUNotificationCenter.h"
#include "EThread.h"

class EPUSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit EPUSocketClient(QTcpSocket* socket = NULL, QObject *parent = 0);
    virtual ~EPUSocketClient();
public:
    QTcpSocket::SocketState ConnectionState(){return m_nConnectionState;}
    void ConnectToDaemon();

    void Handshake();
    void WriteHeader(EPUHeader* header);
signals:
    
public slots:
    void didDisconnectConnection();
    void didReadyRead();
    void bytesWritten(qint64 bytes);
    //void didChangeConnectionState();
    void didConnectToServer();

public:
    //static EThreadReturnValue RunReadDataThread(void* object, EThreadParam param);
    //void internalRunReadDataLoop();
private:
    QSharedPointer<QTcpSocket>      m_pSocket;
    QTcpSocket::SocketState         m_nConnectionState;
    //EThread*                        m_pReadDataThread;
    //bool                            m_bKeepThreadReadDataRunning;
    //bool                            m_bSocketStopped;
};

typedef QSharedPointer<EPUSocketClient> EPUSocketClientPtr;

#endif // EPUSOCKETCLIENT_H
