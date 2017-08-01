#ifndef EPUMAINCONTROLLER_H
#define EPUMAINCONTROLLER_H

#include <QApplication>
//#include <unistd.h>
#include <Windows.h>
#include "EPUMainWindow.h"
#include "EPUCallbackObject.h"
#include "EPUMainView.h"
#include "EPUMaintenanceGuideView.h"
#include "../EPSCommonLib/EPSPrinterController.h"
#include <QTimer>

class EPUMainController: public EPUCallbackObject
{
public:
    EPUMainController();
    virtual ~EPUMainController();

public:
    void ShowMainWindow();
    //EPUMainWindow* GetMainWindow() {return m_pMainWindow;}

    //callback
    void EPUViewDidCloseWithCode(QWidget *epuView, EPUCallbackCode code);
    void EPUViewDisAble();
    void EPUViewDisAbleHeadClean();
    void EPUViewAbleNozzleCheck();
    void EPUViewEnAble();

    ///notify
    void PostNotify(EPUEventType eventType);

    //update
    void UpdateCurrentPrinterStatus();
    void StartMainViewTimer(){m_pMainView->StartTimer();}
    void StopMainViewTimer(){m_pMainView->StopTimer();}
private:
    EPUMainWindow*              m_pMainWindow;
    EPUMainView*                m_pMainView;
    EPUPrinter*                 m_pCurrenPrinter;
};

#endif // EPUMAINCONTROLLER_H
