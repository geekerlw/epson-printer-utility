#ifndef EPUPRINTERCONTROLLER_H
#define EPUPRINTERCONTROLLER_H

#include "../EPUCommon/EThread.h"
#include "EPUPrinter.h"
#include "EPSPrinterController.h"

class EPUPrinterController
{
private:
    EPUPrinterController();
    virtual ~EPUPrinterController();

public:
    static EPUPrinterController* GetInstance();
    static void ReleaseInstance();
    void StartUpdatePrinterStatus();
    static EThreadReturnValue RunRefreshPrinterStatus(void* object, EThreadParam param);
    void runRefreshPrinterStatusLoop();
    void FreeThreadUpdateStatus();

public:
    QVector<EPUPrinter*> GetPrinterList();
    EPUPrinter* GetCurrentPrinter();
    void UpdateCurrentPrinterWithIndex(int index);

private:
    static EPUPrinterController*    pSelf;
    EThread*                        m_pRefreshPrinterStatusThread;
    QMutex                          m_RefreshMutex;
    QVector<EPUPrinter*>            m_printerList;
    bool                            m_bKeepRefreshThreadRunning;
    bool                            m_bRefreshThreadStopped;
    EPUPrinter*                     m_pCurrentPrinter;

public:
    void GetPrinterListFromLib();
};

#endif // EPUPRINTERCONTROLLER_H
