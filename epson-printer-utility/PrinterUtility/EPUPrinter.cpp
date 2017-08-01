#include "EPUPrinter.h"
#include <cups/cups.h>
#include <QDebug>

EPUPrinter::EPUPrinter()
    : m_strPrinterName("")
    , m_strModelName("")
    , m_strPrinterID("")

{
}

EPUPrinter::EPUPrinter(const QString& printerName, const QString& modelName, const QString& printerID,
                       const EPS_INK_INFO &inkInfo, EPS_STATUS &status, EPS_UINT32 &language)
    : m_strPrinterName(printerName)
    , m_strModelName(modelName)
    , m_strPrinterID(printerID)
    , m_inkInfo(inkInfo)
    , m_bPrinterDisconnected(false)
    , m_strPrintCmd(language)
{
    SetPrinterStatus(status);
}

void EPUPrinter::SetPrinterStatus(EPS_STATUS &status)
{
    m_status.errorCode = status.errorCode;
    m_status.jobContinue = status.jobContinue;
    m_status.printerStatus = status.printerStatus;
    m_status.paper_count = status.paper_count;
    qDebug()<<"m_status.errorCode: "<<m_status.errorCode;

}

EPUPrinter::~EPUPrinter()
{

}
