#include "EPUSocketServer.h"
#include "../EPSCommonLib/EPSPrinterController.h"
#include "../EPSCommonLib/EPSCommonLibDefine.h"

EPUSocketServer::EPUSocketServer(QObject *parent)
    : QObject(parent)
{
    m_pTcpServer = new QTcpServer(this);
    EPUNotificationCenter* center = GetNotificationCenter();
    center->AddObserver(this, EPUClientSocketDidDisconnectFromServer, EPUSocketServer::ClientDidDisconnect);
    center->AddObserver(this, EPUClientSocketDidHandShakeToServer, EPUSocketServer::ClientDidConnect);
    center->AddObserver(this, EPUSTMRequestStatusForCurrentPrinter, EPUSocketServer::RequestStatus);
    QObject::connect(m_pTcpServer, SIGNAL(newConnection()), this, SLOT(didReceiveNewConnectionRequest()));
}

void EPUSocketServer::didReceiveNewConnectionRequest()
{
    QTcpSocket* socket = m_pTcpServer->nextPendingConnection();
    if (socket)
    {
        qDebug()<<"process pending connection";
        EPUSocketClient* socketPtr = new EPUSocketClient(socket);
        if (socketPtr->ConnectionState() == QTcpSocket::ConnectedState)
        {
            QMutexLocker locker(&m_sockListMutex);
            m_clientList.push_back(socketPtr);
        }
    }
}

void EPUSocketServer::StartListen()
{
    m_pTcpServer->listen(QHostAddress::LocalHost, DAEMON_LISTEN_PORT);
}

void EPUSocketServer::ClientDidDisconnect(EPUObserver *ptr, EPUObjectPtr object)
{
    EPUSocketServer* pServer = (EPUSocketServer*)ptr;
    if (pServer)
    {
        pServer->internalHandleClientDidDisconnect(object);
    }
}

void EPUSocketServer::internalHandleClientDidDisconnect(EPUObjectPtr object)
{
    EPUObjectPtr ptr = object;
    EPUSocketClient* pClient = (EPUSocketClient*)ptr->Object();
    if (pClient)
    {
        QMutexLocker locker(&m_sockListMutex);
        qDebug()<<"Run in thread: "<<QThread::currentThread()->priority();
        qDebug()<<"Will remove client: "<<pClient;
        int index = m_clientList.indexOf(pClient);
        if (index >= 0 && index < m_clientList.size())
        {
            qDebug()<<"Did remove client: "<<pClient;
            m_clientList.remove(index);
        }
    }
}

void EPUSocketServer::ClientDidConnect(EPUObserver *ptr, EPUObjectPtr object)
{
    EPUSocketServer* pServer = (EPUSocketServer*)ptr;
    if (pServer)
    {
        pServer->internalHandleClientDidConnect(object);
    }
}

void EPUSocketServer::internalHandleClientDidConnect(EPUObjectPtr object)
{
    EPUSocketClient* pClient = (EPUSocketClient*)object->Object();
    if (pClient)
    {
        QMutexLocker locker(&m_sockListMutex);
        qDebug()<<"Run in thread: "<<QThread::currentThread()->priority();
        qDebug()<<"Accept new STM: "<<pClient;
        m_clientList.push_back(pClient);
    }
}

void EPUSocketServer::RequestStatus(EPUObserver* ptr, EPUObjectPtr object)
{
    EPUSocketServer* pServer = (EPUSocketServer*)ptr;
    if (pServer)
    {
        pServer->internalRequestStatus(object);
    }
}

void EPUSocketServer::internalRequestStatus(EPUObjectPtr object)
{
    QString prName = object->StringValue();
    qDebug()<<"Did request status for printer: "<<prName;
    EPSPrinterController* controller = EPSPrinterController::GetInstance();
    if (controller->ConnectToPrinter(prName))
    {
        qDebug()<<"Did connect to printer: "<<prName;
        //get printer status
        EPRP_STATUS status;
        EPRP_INK_INFO inkInfo;
        if (controller->GetCurrentPrinterStatus(&status, &inkInfo))
        {
            qDebug()<<"Did get status for printer: "<<prName;
            EPUHeader header;
            header.commandId = EPU_COMMAND_SEND_STATUS;
            header.bodyLength = sizeof(EPRP_STATUS);
            memcpy(header.bodyData, &status, sizeof(EPRP_STATUS));
            for (int i = 0; i < m_clientList.size(); i++)
            {
                EPUSocketClient* pClient = m_clientList[i];
                pClient->WriteHeader(&header);
            }

            EPUHeader header2;
            header2.commandId = EPU_COMMAND_SEND_INK_INFO;
            header2.bodyLength = sizeof(EPRP_INK_INFO);
            memcpy(header2.bodyData, &inkInfo, sizeof(EPRP_INK_INFO));
            for (int i = 0; i < m_clientList.size(); i++)
            {
                EPUSocketClient* pClient = m_clientList[i];
                pClient->WriteHeader(&header2);


                EPRP_INK_INFO* ink = (EPRP_INK_INFO*)header2.bodyData;
                qDebug()<<"numInks send to client: "<<ink->numInks;
            }
        }
        else
        {
            qDebug()<<"Can't get status for printer: "<<prName;
        }
    }
    else
    {
        qDebug()<<"Can not connect to: "<<prName;
        EPRP_STATUS status;
        status.printerStatus = EPS_PRNERR_COMM;
        EPUHeader header;
        header.commandId = EPU_COMMAND_SEND_STATUS;
        header.bodyLength = sizeof(EPRP_STATUS);
        memcpy(header.bodyData, &status, sizeof(EPRP_STATUS));
        for (int i = 0; i < m_clientList.size(); i++)
        {
            EPUSocketClient* pClient = m_clientList[i];
            pClient->WriteHeader(&header);
        }
    }
}



