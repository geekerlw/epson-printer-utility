#include "EPUSocketClient.h"
#include "../EPSCommonLib/EPSCommonLibDefine.h"

EPUSocketClient::EPUSocketClient(QTcpSocket* socket, QObject *parent)
    : QObject(parent)
    //, m_pReadDataThread(NULL)
    //, m_bKeepThreadReadDataRunning(true)
    //, m_bSocketStopped(false)
{
    m_nConnectionState = QTcpSocket::UnconnectedState;
    if (socket)
    {
        QSharedPointer<QTcpSocket> sockPtr(socket);
        m_pSocket = sockPtr;
        m_nConnectionState = QTcpSocket::ConnectedState;

        QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(didDisconnectConnection()));
        QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(didReadyRead()));
        QObject::connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));

        //m_pReadDataThread = EThreadCreateAndRun(EPUSocketClient::RunReadDataThread, (void*)this, EThreadParam());
    }
    else
    {
        QTcpSocket* pSock = new QTcpSocket(this);
        m_pSocket = QSharedPointer<QTcpSocket>(pSock);

        QObject::connect(pSock, SIGNAL(disconnected()), this, SLOT(didDisconnectConnection()));
        QObject::connect(pSock, SIGNAL(readyRead()), this, SLOT(didReadyRead()));
        QObject::connect(pSock, SIGNAL(connected()), this, SLOT(didConnectToServer()));
        QObject::connect(pSock, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    }
    //m_pSocket->setReadBufferSize(1024);
}

EPUSocketClient::~EPUSocketClient()
{
    qDebug()<<"dealloc socket client";
    /*m_bKeepThreadReadDataRunning = false;
    while (!m_bSocketStopped)
    {
        EThread::msleep(100);
    }
    if (m_pReadDataThread)
    {
        EThreadFree(m_pReadDataThread);
        m_pReadDataThread = NULL;
    }*/
    if (m_pSocket)
    {
        m_pSocket->deleteLater();
    }
}

void EPUSocketClient::didDisconnectConnection()
{
    qDebug()<<"******************did disconnect connection";
    EPUNotificationCenter* center = GetNotificationCenter();
    QObject::disconnect(m_pSocket.data(), SIGNAL(disconnected()), this, SLOT(didDisconnectConnection()));
    QObject::disconnect(m_pSocket.data(), SIGNAL(readyRead()), this, SLOT(didReadyRead()));
    QObject::disconnect(m_pSocket.data(), SIGNAL(connected()), this, SLOT(didConnectToServer()));
    center->PostNotification(EPUClientSocketDidDisconnectFromServer, EPUObjectPtr(new EPUObject(this)));
}

/*EThreadReturnValue EPUSocketClient::RunReadDataThread(void *object, EThreadParam param)
{
    EPUSocketClient* client = (EPUSocketClient*)object;
    if (client)
    {
        client->internalRunReadDataLoop();
    }
    return 0;
}*/

//void EPUSocketClient::internalRunReadDataLoop()
void EPUSocketClient::didReadyRead()
{
    EPUHeader header;
    EPUNotificationCenter* center = GetNotificationCenter();
    qint64 bytesAvailable = m_pSocket->bytesAvailable();
    qDebug()<<"bytesAvailable: "<<bytesAvailable;
    qDebug()<<"socket header size: "<<sizeof(header);
    if (m_pSocket->bytesAvailable() >= sizeof(header))
    {
        int len = m_pSocket->read((char*)&header, sizeof(header));
        if (len == sizeof(header))
        {
            qDebug()<<"Command Id: "<<header.commandId;
            switch (header.commandId)
            {
            case EPU_COMMAND_ACK:
            {
                center->PostNotification(EPUClientSocketDidHandShakeToServer, EPUObjectPtr(new EPUObject(this)));
                break;
            }
            case EPU_COMMAND_GET_STATUS:
            {
                const char* prName = (const char*)header.bodyData;
                QString prNameStr = QString(prName);
                center->PostNotification(EPUSTMRequestStatusForCurrentPrinter, EPUObjectPtr(new EPUObject(prNameStr)));
                break;
            }
            case EPU_COMMAND_SEND_STATUS:
            {
                EPRP_STATUS status;
                memcpy(&status, header.bodyData, header.bodyLength);
                qDebug()<<"Did receive printer status, post notification status: "<<status.printerStatus;
                center->PostNotification(EPUSTMDidReceiveStatusForPrinter, EPUObjectPtr(new EPUObject(status)));
                break;
            }
            case EPU_COMMAND_SEND_INK_INFO:
            {
                EPRP_INK_INFO inkInfo;
                memcpy(&inkInfo, header.bodyData, header.bodyLength);
                qDebug()<<"Did receive ink info, post notification";
                center->PostNotification(EPUSTMDidReceiveInkInfoForPrinter, EPUObjectPtr(new EPUObject(inkInfo)));
                break;
            }
            default:
            {
                m_pSocket->disconnect();
                break;
            }
            }
        }
        else
        {
            qDebug()<<"Can't get full packet length";
            m_pSocket->disconnect();
        }
    }
    else
    {
        qDebug()<<"No data to read";
    }
}

void EPUSocketClient::didConnectToServer()
{
    qDebug()<<"Did connect to daemon";
    m_nConnectionState = QTcpSocket::ConnectedState;
    //m_pReadDataThread = EThreadCreateAndRun(EPUSocketClient::RunReadDataThread, (void*)this, EThreadParam());
    Handshake();
    EPUNotificationCenter* center = GetNotificationCenter();
    center->PostNotification(EPUClientSocketDidConnectToDaemon, EPUObjectPtr(new EPUObject(this)));
}

void EPUSocketClient::ConnectToDaemon()
{
    if (m_pSocket)
    {
        m_pSocket->connectToHost(QHostAddress::LocalHost, DAEMON_LISTEN_PORT);
    }
}

void EPUSocketClient::Handshake()
{
    EPUHeader header;
    header.commandId = EPU_COMMAND_ACK;
    header.bodyLength = sizeof(EPUHeader);

    qDebug()<<"Send ACK comand";
    WriteHeader(&header);
    qDebug()<<"Did send ACK command";
}

void EPUSocketClient::bytesWritten(qint64 bytes)
{
    qDebug()<<"did write: "<<bytes<< " bytes";
}

void EPUSocketClient::WriteHeader(EPUHeader* header)
{
    if (m_pSocket)
    {
        //QByteArray bytesArray;
        //QDataStream out(&bytesArray, QIODevice::WriteOnly);
        //out.writeBytes((const char*)header, sizeof(EPUHeader));
        m_pSocket->write((const char*)header, sizeof(EPUHeader));
        //m_pSocket->write(bytesArray);
        m_pSocket->flush();
    }
}


