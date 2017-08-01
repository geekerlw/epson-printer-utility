#include "EPSPrinterController.h"
#include <QDebug>

EPSPrinterController* EPSPrinterController::pSelf = NULL;

EPSPrinterController::EPSPrinterController()
    : m_bIsInitialized(false)
    , m_bCommunicationIsBusy(false)
{
}

EPSPrinterController::~EPSPrinterController()
{

}

EPSPrinterController* EPSPrinterController::GetInstance()
{
    if (!pSelf)
    {
        pSelf = new EPSPrinterController();
        //pSelf->InitializeCommonLib();
    }
    return pSelf;
}

void EPSPrinterController::ReleaseInstance()
{
    if (pSelf)
    {
        pSelf->ReleaseCommonLib();
        delete pSelf;
        pSelf = NULL;
    }
}

bool EPSPrinterController::InitializeCommonLib()
{
    EPSCommunicator* communicator = EPSCommunicator::GetInstance();
    if (!communicator->InitializeCommunicator())
    {
        return false;
    }
    m_bIsInitialized = true;

    return m_bIsInitialized;
}

void EPSPrinterController::ReleaseCommonLib()
{
    EPSCommunicator::ReleaseInstance();
}

void EPSPrinterController::StartFindPrinter()
{
    //debug_msg("Start call Function EPSPrinterController::StartFindPrinter \n");
    EPSCommunicator* communicator = EPSCommunicator::GetInstance();
    communicator->StartFindPrinter();
    //debug_msg("Exit call Function EPSPrinterController::StartFindPrinter \n");
}

EPS_ERR_CODE EPSPrinterController::NozzleCheckPrinter()
{
    EPS_ERR_CODE ret = EPS_ERR_NONE;

    EPSCommunicator* communicator = EPSCommunicator::GetInstance();
    ret = communicator->NozzleCheckPrinter();

    if(ret != EPS_ERR_NONE){
        qDebug()<<"Can not Nozzle check Printer err: "<<ret;
    }

    return ret;
}

bool EPSPrinterController::ConnectToPrinter(const QString &printerName, const QString &printerId)
{
    EPSCommunicator* communicator = EPSCommunicator::GetInstance();
    return communicator->ConnectToPrinter(printerName, printerId);
}

bool EPSPrinterController::GetPrinterStatus(EPS_STATUS* status)
{
    EPSCommunicator* communicator = EPSCommunicator::GetInstance();
    return communicator->GetPrinterStatus(status);
}

EPS_ERR_CODE EPSPrinterController::HeadCleanPrinter()
{
    EPSCommunicator* communicator = EPSCommunicator::GetInstance();
    return communicator->HeadCleanPrinter();
}

EPS_INT32 EPSPrinterController::getSizeOfPrinterList()
{
    EPSCommunicator* communicator = EPSCommunicator::GetInstance();
    return communicator->getSizeOfPrinterList();
}

QVector<EPS_PRINTER> EPSPrinterController::GetPrinterList()
{
    EPSCommunicator* communicator = EPSCommunicator::GetInstance();
    return communicator->GetPrinterList();
}

bool EPSPrinterController::GetCurrentPrinterStatus(EPS_STATUS *status, EPS_INK_INFO* inkInfo)
{
    EPSCommunicator* communicator = EPSCommunicator::GetInstance();
    bool ret = false;
    if (!m_bCommunicationIsBusy)
    {
        m_bCommunicationIsBusy = true;
        ret = communicator->GetCurrentPrinterStatus(status, inkInfo);
        m_bCommunicationIsBusy = false;
    }
    else
    {
        qDebug()<<"communication is busy";
    }
    return ret;
}




