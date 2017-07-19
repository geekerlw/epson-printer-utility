#ifndef EPSCOMMUNICATOR_H
#define EPSCOMMUNICATOR_H

#include "epson-escpr-media.h"
#include "epson-cbt.h"
#include "epson-protocol.h"
#include "epson-net-snmp.h"
#include "epson-net-lpr.h"
#include "epson-net-lpr.h"
#include "epson-usb.h"
//#include "linux_cmn.h"
//#include "linux_net.h"
#include "epson-protocol.h"
//#include "epsmp-common-functions.h"
#include "epsmp-linux-portal-io.h"
#include "epsmp-file-io.h"
#include "epson-escpr-api.h"
#include "epson-escpr-def.h"

#include "../EPUCommon/EThread.h"
#include "EPSCommonLibDefine.h"

class EPSPrinterController;

class EPSCommunicator
{
private:
    EPSCommunicator();
    virtual ~EPSCommunicator();

public:
    static EPSCommunicator* GetInstance();
    static void ReleaseInstance();

public:
    bool InitializeCommunicator();
    void StartFindPrinter();

    void SetPrinterController(EPSPrinterController* pController) {m_pPrinterController = pController;}

    //QVector<EPSPrinter> GetPrinterList();
    QVector<EPS_PRINTER> GetPrinterList(){return m_printerList;}

    static EPS_INT32 FindPrinterCallback(EPS_PRINTER printer);
    static EThreadReturnValue RunFindPrinterThread(void* object, EThreadParam param);
    EPS_INT32 didFindPrinter(EPS_PRINTER printer);
    void internalRunFindPrinterLoop();

    bool ConnectToPrinter(const QString& printerName, const QString &printerId);
    bool GetCurrentPrinterStatus(EPS_STATUS* status, EPS_INK_INFO* inkInfo);

    EPS_ERR_CODE NozzleCheckPrinter();
    EPS_ERR_CODE HeadCleanPrinter();
    EPS_INT32 getSizeOfPrinterList(){return m_printerList.size();}
    bool GetPrinterStatus(EPS_STATUS* status);

private:
    static EPSCommunicator* pSelf;
    EThread*                m_pFindPrinterThread;
    bool                    m_bKeepFindPrinterThread;
    EPSPrinterController*   m_pPrinterController;

    QVector<EPS_PRINTER>  m_printerList;
    EPS_PRINTER           m_currentPrinter;
    APP_SEARCH_ATTRIBS    m_searchAttr;
};

#endif // EPSCOMMUNICATOR_H
