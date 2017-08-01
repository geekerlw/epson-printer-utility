#ifndef EPSCOMMONLIB_H
#define EPSCOMMONLIB_H

#include "EPSCommonLibDefine.h"
#include "EPSCommunicator.h"

class EPSPrinterController
{
private:
    EPSPrinterController();
    virtual ~EPSPrinterController();

public:
    static EPSPrinterController* GetInstance();
    static void ReleaseInstance();

public:
    bool InitializeCommonLib();
    bool IsInitializedCommLib() {return m_bIsInitialized;}
    void ReleaseCommonLib();
    void StartFindPrinter();

    bool ConnectToPrinter(const QString& printerName, const QString &printerId);
    bool GetCurrentPrinterStatus(EPS_STATUS *status, EPS_INK_INFO* inkInfo);
    EPS_ERR_CODE NozzleCheckPrinter();
    EPS_ERR_CODE HeadCleanPrinter();
    EPS_INT32 getSizeOfPrinterList();
    QVector<EPS_PRINTER> GetPrinterList();
    bool GetPrinterStatus(EPS_STATUS* status);

private:
    static EPSPrinterController*    pSelf;
    bool                            m_bIsInitialized;
    bool                            m_bCommunicationIsBusy;
};

#endif // EPSCOMMONLIB_H
