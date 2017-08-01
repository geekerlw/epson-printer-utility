#include "EPUMainController.h"
#include "EPUPrinterController.h"
#include <QTimer>
//#include "err.h"

EPUMainController::EPUMainController()
{
    //debug_msg("Start Call Function EPUMainController \n");
    m_pMainWindow = new EPUMainWindow();

    EPSPrinterController* controlLib = EPSPrinterController::GetInstance();
    if(controlLib->InitializeCommonLib()){
        controlLib->StartFindPrinter();
    }else{
        qDebug()<<"Can not init library";
    }




    EPUPrinterController* controller = EPUPrinterController::GetInstance();   
    controller->GetPrinterListFromLib();

    m_pMainView = new EPUMainView;
    m_pMainView->SetDelegate(this);
    EPUPrinter* currentPrinter = controller->GetCurrentPrinter();
    if(!currentPrinter)
    {
        qDebug()<<"Can not Find Printer";
        m_pMainView->StatusLabelEmptyPrinterList();
        m_pMainWindow->ButtonDisable();

    }else{
        m_pCurrenPrinter = currentPrinter;
        m_pMainView->updateCurrenPrinterStaus();

    }
    m_pMainWindow->ChangeCurrentViewToView(m_pMainView);
   // debug_msg("Exit Call Function EPUMainController \n");
}

EPUMainController::~EPUMainController()
{
	Sleep(1000);
    SAFEDELETE(m_pMainView);
    SAFEDELETE(m_pMainWindow);
    EPUPrinterController::ReleaseInstance();
    EPSPrinterController::ReleaseInstance();

}

void EPUMainController::ShowMainWindow()
{
    if (m_pMainWindow)
    {
        m_pMainWindow->show();
    }
}

void EPUMainController::EPUViewDisAble()
{
    m_pMainWindow->ButtonDisable();
}

void EPUMainController::EPUViewDisAbleHeadClean()
{
    m_pMainWindow->HeadCleanButtonDisable();
}

void EPUMainController::EPUViewAbleNozzleCheck()
{
    m_pMainWindow->NozzleCheckButtonAble();
}

void EPUMainController::EPUViewEnAble()
{
    m_pMainWindow->ButtonEnable();
}

void EPUMainController::EPUViewDidCloseWithCode(QWidget *epuView, EPUCallbackCode code)
{
    switch (code)
    {
    case EPU_CALLBACK_NONE:
    case EPU_CALLBACK_FINISH:
    {
        m_pMainWindow->ChangeCurrentViewToView(m_pMainView);
        if (epuView == m_pMainWindow->getMaintenanceGuideView())
        {
            StartMainViewTimer();
            //m_pMainView->updateCurrenPrinterStaus();
        }
        qDebug()<<"Callback finish!";
        break;
    }
    case EPU_CALLBACK_GET_NEXT_VIEW:
    {
        /*if (epuView == m_pMainWindow->getMaintenanceGuideView())
        {
            EPUMaintenanceGuideView* guideView = (EPUMaintenanceGuideView*)epuView;
            if (guideView->GetMaintenanceCommand() == EPU_MAINTENANCE_COMMAND_NOZZLE_CHECK)
            {
                m_pMainWindow->ChangeCurrentViewToView(m_pNozzleCheckResultView);
            }
            else if (guideView->GetMaintenanceCommand() == EPU_MAINTENANCE_COMMAND_HEAD_CLEANING)
            {
                m_pMainWindow->ChangeCurrentViewToView(m_pHeadCleaningView);
            }
        }*/
        break;
    }
    default:
    {
        break;
    }
    }
}

void EPUMainController::PostNotify(EPUEventType eventType)
{
    QApplication::postEvent(m_pMainWindow, new QEvent((QEvent::Type)eventType));
}

void EPUMainController::UpdateCurrentPrinterStatus()
{
    EPUPrinterController* controller = EPUPrinterController::GetInstance();

    if(m_pCurrenPrinter->GetPrinterId() == controller->GetCurrentPrinter()->GetPrinterId())
    {
        m_pMainView->updatePrinterStatus();
    }else
    {
        m_pCurrenPrinter = controller->GetCurrentPrinter();
    }
}

