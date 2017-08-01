#ifndef EPUPRINTER_H
#define EPUPRINTER_H

#include <QString>
#include "../EPUCommon/EPUCommonDef.h"

enum
{
    EPU_PRINTER_BUSY = 2,
    EPU_PRINTER_IDLE = 3,
    EPU_PRINTER_PRINTING,
    EPU_PRINTER_STOPPED
};

typedef int EPU_PRINTER_STATE;

class EPUPrinter
{
public:
    EPUPrinter();
    EPUPrinter(const QString& printerName, const QString& modelName, const QString& printerID,
               const EPS_INK_INFO &inkInfo, EPS_STATUS &status, EPS_UINT32 &language);
    virtual ~EPUPrinter();

public:
    QString GetPrinterName(){return m_strModelName;}
    EPS_UINT32 GetPrintCmd(){return  m_strPrintCmd;}

    QString GetPrinterId(){return m_strPrinterID;}
    QString GetModelName(){return m_strModelName;}

    EPS_INK_INFO GetInkInfo(){return m_inkInfo;}
    void SetInkInfo(EPS_INK_INFO inkInfo){m_inkInfo = inkInfo;}
    void SetPrinterDisconnected(bool value){m_bPrinterDisconnected = value;}
    bool GetPrinterDisconnected(){return m_bPrinterDisconnected;}
    void SetPrinterStatus(EPS_STATUS &status);
    EPS_STATUS GetPrinterStatus(){return m_status;}

private:
    bool                         m_bPrinterDisconnected;
    QString                      m_strPrinterName;
    QString                      m_strPrinterID;
    EPS_UINT32                   m_strPrintCmd;
    EPS_INK_INFO                m_inkInfo;
    EPS_STATUS                  m_status;
    QString                      m_strModelName;
};

#endif // EPUPRINTER_H
