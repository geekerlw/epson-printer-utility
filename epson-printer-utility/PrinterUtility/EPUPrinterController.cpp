#include "EPUPrinterController.h"
#include "../EPUCommon/EPUCommonDef.h"
#include "EPUGlobal.h"

EPUPrinterController* EPUPrinterController::pSelf = NULL;

EPUPrinterController::EPUPrinterController()
    : m_bKeepRefreshThreadRunning(true)
    , m_pRefreshPrinterStatusThread(NULL)
    ,m_bRefreshThreadStopped(true)
{
    m_pCurrentPrinter = NULL;
}

EPUPrinterController::~EPUPrinterController()
{
    m_bKeepRefreshThreadRunning = false;
    if (m_pRefreshPrinterStatusThread)
    {
        EThreadFree(m_pRefreshPrinterStatusThread);
        m_pRefreshPrinterStatusThread = NULL;
    }
}

EPUPrinterController* EPUPrinterController::GetInstance()
{
    if (!pSelf)
    {
        pSelf = new EPUPrinterController();
    }
    return pSelf;
}

void EPUPrinterController::ReleaseInstance()
{
    if (pSelf)
    {
        delete pSelf;
        pSelf = NULL;
    }
}

EThreadReturnValue EPUPrinterController::RunRefreshPrinterStatus(void* object, EThreadParam param)
{
    EPUPrinterController* controller = (EPUPrinterController*)object;
    if (controller)
    {
        controller->runRefreshPrinterStatusLoop();
    }
    return 0;
}

void EPUPrinterController::runRefreshPrinterStatusLoop()
{
    m_bKeepRefreshThreadRunning = true;
    m_bRefreshThreadStopped = false;

    EPSPrinterController* controller = EPSPrinterController::GetInstance();
    if (m_pCurrentPrinter)
    {
        EPUPrinter* printer = m_pCurrentPrinter;
        if (controller->ConnectToPrinter(m_pCurrentPrinter->GetPrinterName(), m_pCurrentPrinter->GetPrinterId()))
        {
            EPS_INK_INFO ink;
            EPS_STATUS status;
            if (controller->GetCurrentPrinterStatus(&status, &ink))
            {

                if(printer->GetPrinterId() == m_pCurrentPrinter->GetPrinterId())
                {
                    m_pCurrentPrinter->SetPrinterStatus(status);
                    m_pCurrentPrinter->SetInkInfo(ink);
                    m_pCurrentPrinter->SetPrinterDisconnected(false);
                }
            }else{
                if(printer->GetPrinterId() == m_pCurrentPrinter->GetPrinterId())
                {
                    m_pCurrentPrinter->SetPrinterDisconnected(true);
                }
            }
            GetMainController()->PostNotify((QEvent::Type)EPU_EVENT_TYPE_UPDATE_STATUS);
        }else
        {
            m_pCurrentPrinter->SetPrinterDisconnected(true);
        }
    }
    m_bKeepRefreshThreadRunning = false;
    m_bRefreshThreadStopped = true;
}

void EPUPrinterController::StartUpdatePrinterStatus()
{

    qDebug()<<"Check call timer";
    if (m_bRefreshThreadStopped)
    {
        if (m_pRefreshPrinterStatusThread)
        {
            qDebug() << "StartUpdatePrinterStatus free m_pRefreshPrinterStatusThread";
            EThreadFree(m_pRefreshPrinterStatusThread);
            m_pRefreshPrinterStatusThread = NULL;
        }
        m_pRefreshPrinterStatusThread = EThreadCreateAndRun(EPUPrinterController::RunRefreshPrinterStatus, (void*)this, EThreadParam());
    }
}

void EPUPrinterController::FreeThreadUpdateStatus()
{
    qDebug()<<"check call function FreeThreadUpdateStatus";
    //qDebug() << "m_pRefreshPrinterStatusThread = " << m_pRefreshPrinterStatusThread;
    if (m_pRefreshPrinterStatusThread != NULL)
    {
        EThreadFree(m_pRefreshPrinterStatusThread);
        m_pRefreshPrinterStatusThread = NULL;
    }
}

void EPUPrinterController::GetPrinterListFromLib()
{
    QString printerName = QString::null;
    QString modelName = QString::null;
    QString printerId = QString::null;
    EPS_UINT32 language = 0;
    EPS_INK_INFO inkInfo;
    EPS_STATUS status;
    QVector<EPUPrinter*> lst;

    EPSPrinterController* controlLib = EPSPrinterController::GetInstance();
    QVector<EPS_PRINTER> prList = controlLib->GetPrinterList();
    qDebug()<<"prList size: "<<prList.size();
    qDebug()<<"lst.size ===== "<<lst.size();  
    if(prList.size() > 0)
    {
        for(int i = 0; i < prList.size(); i++)
        {
            bool ret = false;
            printerName = QString(prList[i].modelName);
            modelName = QString(prList[i].modelName);
            printerId = QString(prList[i].printerID);
            language = prList[i].language;
            qDebug()<<"printer id: "<<printerId;
            if(controlLib->ConnectToPrinter(printerName, printerId))
            {
                qDebug()<<"check connect to Printer:"<<printerName;
                ret = controlLib->GetCurrentPrinterStatus(&status, &inkInfo);
            }
			if(!ret)
			{
				inkInfo.number = 4;
				for(int i = 0; i < 4; i++)
				{
					inkInfo.remaining[i] = 0;
				}
				inkInfo.colors[0] = EPS_COLOR_BLACK;
				inkInfo.colors[1] = EPS_COLOR_CYAN;
				inkInfo.colors[2] = EPS_COLOR_MAGENTA;
				inkInfo.colors[3] = EPS_COLOR_YELLOW;
			}
            /*if(ret)
            {*/
			qDebug()<<"Check push back printer to List";
			EPUPrinter* printer = new EPUPrinter(printerName, modelName, printerId, inkInfo, status, language);
			if(!ret)
			{
				printer->SetPrinterDisconnected(true);
			}
			lst.push_back(printer);
            /*}
            else */if(m_pCurrentPrinter)
            {
                if(m_pCurrentPrinter->GetModelName() == modelName &&
                   m_pCurrentPrinter->GetPrinterId() == printerId)
                {
                    m_pCurrentPrinter = NULL;
                }
            }
        }
    }
    for(int i = 0; i < lst.size(); i++)
    {
        qDebug()<<"--------Printer Name--------:"<<lst[i]->GetModelName();
    }
    /////locker
    {
        QMutexLocker locker(&m_RefreshMutex);
        m_printerList = lst;
    }

    if(lst.size() > 0)
    {
        if (!m_pCurrentPrinter)
        {
            qDebug()<<"Curren Printer = NULL";
            m_pCurrentPrinter = lst[0];
            qDebug()<<"Curren Printer: "<<m_pCurrentPrinter->GetModelName();
        }
        else
        {
            qDebug()<<"Curren Printer != NULL";
            EPUPrinter* ptr = GetCurrentPrinter();
            m_pCurrentPrinter = ptr;
        }
    }
    else
    {
        m_pCurrentPrinter = NULL;
    }
    if(m_pCurrentPrinter)
    {
        controlLib->ConnectToPrinter(m_pCurrentPrinter->GetModelName(), m_pCurrentPrinter->GetPrinterId());
    }
}

QVector<EPUPrinter*> EPUPrinterController::GetPrinterList()
{
    QVector<EPUPrinter*> lst;
    //for release lock
    {
        QMutexLocker locker(&m_RefreshMutex);
        lst = m_printerList;
    }
    return lst;
}

EPUPrinter* EPUPrinterController::GetCurrentPrinter()
{
    QVector<EPUPrinter*> lst = GetPrinterList();
    qDebug()<<"List size: "<<lst.size();
    if(!lst.isEmpty())
    {
        for (int i = 0; i < lst.size(); i++)
        {
            EPUPrinter* pr = lst[i];
            if (m_pCurrentPrinter)
            {
                if (pr->GetPrinterName() == m_pCurrentPrinter->GetPrinterName())
                {
                    m_pCurrentPrinter = pr;
                    return m_pCurrentPrinter;
                }
            }
        }
    }
    m_pCurrentPrinter = NULL;
    return m_pCurrentPrinter;
}

void EPUPrinterController::UpdateCurrentPrinterWithIndex(int index)
{
    QVector<EPUPrinter*> lst = GetPrinterList();
    if (index >= 0 && index < lst.size())
    {
        m_pCurrentPrinter = lst[index];
    }
}




