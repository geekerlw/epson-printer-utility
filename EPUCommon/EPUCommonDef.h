#ifndef EPUCOMMONDEF_H
#define EPUCOMMONDEF_H

#include <QtGui>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
//#include <QSharedPointer>

#define SAFEDELETE(a)       if (a)          \
                                delete a;   \
                            a = NULL

#define DAEMON_LISTEN_PORT                          4000

#define EPUClientSocketDidDisconnectFromServer      "EPUClientSocketDidDisconnectFromServer"
#define EPUClientSocketDidHandShakeToServer         "EPUClientSocketDidHandShakeToServer"
#define EPUDidUpdatePrinterListNotification         "EPUDidUpdatePrinterListNotification"
#define EPUDidUpdateCurrentPrinter                  "EPUDidUpdateCurrentPrinter"
#define EPUClientSocketDidConnectToDaemon           "EPUClientSocketDidConnectToDaemon"

///for daemon
#define EPUSTMRequestStatusForCurrentPrinter        "EPUSTMRequestStatusForCurrentPrinter"
#define EPUSTMDidReceiveStatusForPrinter            "EPUSTMDidReceiveStatusForPrinter"
#define EPUSTMDidReceiveInkInfoForPrinter           "EPUSTMDidReceiveInkInfoForPrinter"

enum
{
    EPU_ERR_OK              = 0,
    EPU_ERR_NONE            = 1,

    EPU_ERR_FAILURE         = 0x80000000
};

typedef qint32              EPUErrCode;

enum
{
    EPU_EVENT_TYPE_UPDATE_STATUS        = QEvent::User + 1,
    EPU_EVENT_TYPE_UPDATE_PRINTER_LIST
};

typedef qint32              EPUEventType;

inline bool EPUSucceeded(EPUErrCode code)
{
    return (EPU_ERR_OK == code);
}

inline QString EPUErrString(EPUErrCode code)
{
    QString ret = "";
    switch (code)
    {
    case EPU_ERR_OK:
    {
        ret = "EPU_ERR_OK";
        break;
    }
    case EPU_ERR_NONE:
    {
        ret = "EPU_ERR_NONE";
        break;
    }
    case EPU_ERR_FAILURE:
    {
        ret = "EPU_ERR_FAILURE";
        break;
    }
    default:
        break;
    }
    return ret;
}

enum
{
    EPU_COMMAND_ACK = 100,
    EPU_COMMAND_GET_STATUS,

    ///reply
    EPU_COMMAND_SEND_STATUS,
    EPU_COMMAND_SEND_INK_INFO
};

typedef quint8 EPUCommandId;

#define MAX_PACKET_BODY_LEN     256
typedef struct _EPUHeader
{
    EPUCommandId commandId;
    quint8       bodyLength;
    qint8        bodyData[MAX_PACKET_BODY_LEN];
    _EPUHeader()
    {
        commandId = 0;
        bodyLength = 0;
        memset(bodyData, 0x00, sizeof(MAX_PACKET_BODY_LEN));
    }
} EPUHeader;

#endif // EPUCOMMONDEF_H








