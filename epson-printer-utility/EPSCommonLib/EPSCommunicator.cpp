#include "EPSCommunicator.h"
#include <QDebug>

#include "../BackendLib/backend_api.h"


EPSCommunicator* EPSCommunicator::pSelf = NULL;

EPSCommunicator::EPSCommunicator()
    : m_pFindPrinterThread(NULL)
    , m_bKeepFindPrinterThread(true)
    , m_pPrinterController(NULL)
{


}

EPSCommunicator::~EPSCommunicator()
{
    if (m_pFindPrinterThread)
    {
        m_bKeepFindPrinterThread = false;
        EThreadFree(m_pFindPrinterThread);
        m_pFindPrinterThread = NULL;

		/* Close Library (Disconnect from USB backend) */
		Close_Lib ();

    }

}

EPSCommunicator* EPSCommunicator::GetInstance()
{

    if (!pSelf)
    {
        pSelf = new EPSCommunicator();
    }
    return pSelf;
}

void EPSCommunicator::ReleaseInstance()
{

    if (pSelf)
    {
        delete pSelf;
        pSelf = NULL;
//        epsReleaseDriver();
    }

}

bool EPSCommunicator::InitializeCommunicator()
{
#if 0
    int ret = -1;
    EPS_ERR_CODE    retStatus = 0;              /* Return status of internal calls      */
    APP_SETTING		stAppSetting;
    EPS_CMN_FUNC	externCmnFuncPtr;			/* External common function pointers	*/
    EPS_NET_FUNC	externNetFuncPtr;			/* External net function pointers		*/
    EPS_USB_FUNC	externUsbFuncPtr;           /* External usb function pointers       */

    InitMutex();

    memset(&stAppSetting, 0, sizeof(stAppSetting));
    //stAppSetting.nRunMode  = SMP_RUNMODE_CUI;					/* Interactive		*/
    //stAppSetting.nFindMode = SMP_FINDMODE_ALL;					/* Find all printer */
    stAppSetting.nComMode  = EPS_COMM_BID;	                    /* bi direction     */

#ifdef GCOMSW_PRT_USE_USB
    stAppSetting.nComMode  |= EPS_PROTOCOL_USB;
#endif
#ifdef GCOMSW_PRT_USE_LPR
    stAppSetting.nComMode  |= EPS_PROTOCOL_LPR;
#endif

    //stAppSetting.nFindTimeout = SMP_SEARCH_TIME;				/* 10sec            */
    stAppSetting.bAutoScaling = TRUE;							/* On               */
    stAppSetting.bAutoRotation = TRUE;							/* On               */
    stAppSetting.bAPF      = TRUE;								/* On               */
    stAppSetting.nJpegMaxSize = EPS_JPEG_SIZE_MAX;				/* default          */

/*--------------------------------------------------------------------------------------*/
/*** Initialize ESC/P-R                                                                 */
/*--------------------------------------------------------------------------------------*/
    memset(&externUsbFuncPtr, 0, sizeof(externUsbFuncPtr));
    memset(&externNetFuncPtr, 0, sizeof(externNetFuncPtr));
    memset(&externCmnFuncPtr, 0, sizeof(externCmnFuncPtr));
    //epsmpSetExternFuncs(stAppSetting.nComMode, &externUsbFuncPtr, &externNetFuncPtr, &externCmnFuncPtr);
#ifdef GCOMSW_PRT_USE_USB
    externUsbFuncPtr.version           = EPS_USBFUNC_VER_CUR;
    externUsbFuncPtr.openPortal		= &epsmpOpenPortal;
    externUsbFuncPtr.closePortal		= &epsmpClosePortal;
    externUsbFuncPtr.readPortal		= &epsmpReadPortal;
    externUsbFuncPtr.writePortal		= &epsmpWritePortal;
    externUsbFuncPtr.findFirst			= &epsmpFindFirst;
    externUsbFuncPtr.findNext			= &epsmpFindNext;
    externUsbFuncPtr.findClose			= &epsmpFindClose;
    externUsbFuncPtr.getDeviceID	    = &epsmpGetDeviceID;
    externUsbFuncPtr.softReset         = &epsmpSoftReset;
//#if LCOMSW_FILE_IO
    if( !EPS_IS_BI_PROTOCOL(stAppSetting.nComMode) ){
        qDebug()<<"log 0000";
        externUsbFuncPtr.openPortal     = &epsmpOpenFile;
        externUsbFuncPtr.closePortal    = &epsmpCloseFile;
        externUsbFuncPtr.readPortal     = &dummyReadFile;
        externUsbFuncPtr.writePortal    = &epsmpWriteFile;
    }
//#endif
#endif

#if defined(GCOMSW_PRT_USE_LPR) || defined(GCOMSW_PRT_USE_RAW)
    externNetFuncPtr.version           = EPS_NETFUNC_VER_CUR;
    externNetFuncPtr.socket			= &epsmpSocket;
    externNetFuncPtr.close				= &epsmpClose;
    externNetFuncPtr.connect			= &epsmpConnect;
    externNetFuncPtr.shutdown			= &epsmpShutdown;
    externNetFuncPtr.send				= &epsmpSend;
    externNetFuncPtr.sendTo			= &epsmpSendTo;
    externNetFuncPtr.receive			= &epsmpReceive;
    externNetFuncPtr.receiveFrom		= &epsmpReceiveFrom;
    externNetFuncPtr.setMulticastTTL	= &epsmpSetMulticastTTL;
    externNetFuncPtr.setBroadcast		= &epsmpSetBroadCast;
    externNetFuncPtr.bind				= NULL;
    externNetFuncPtr.listen			= NULL;
    externNetFuncPtr.accept			= NULL;
    externNetFuncPtr.getsockname		= NULL;
#endif
    externCmnFuncPtr.version           = EPS_CMNFUNC_VER_CUR;
    externCmnFuncPtr.findCallback		= &FindPrinterCallback;
    externCmnFuncPtr.memAlloc			= &epsmpMemAlloc;
    externCmnFuncPtr.memFree			= &epsmpMemFree;
    externCmnFuncPtr.sleep				= &epsmpSleep;
    externCmnFuncPtr.getTime			= &epsmpGetTime;
    externCmnFuncPtr.getLocalTime 	    = &epsmpGetLocalTime;
    externCmnFuncPtr.lockSync			= &epsmpLockSync;
    externCmnFuncPtr.unlockSync		= &epsmpUnlockSync;
    externCmnFuncPtr.stateCallback		= NULL;	/* current version unused */
    retStatus = epsInitDriver(stAppSetting.nComMode, &externUsbFuncPtr, &externNetFuncPtr, &externCmnFuncPtr);
    if (retStatus != EPS_ERR_NONE){
        //out_printf("ERROR: epsInitDriver returned with error: %d\n", retStatus);
        //goto APP_END;
        qDebug()<<"epsInitDriver returned with error: "<<retStatus;
    }

    m_searchAttr.probe.version = EPS_PROBE_VER_CUR;
    m_searchAttr.probe.dsc.addr.protocol = EPS_PRT_PROTOCOL(stAppSetting.nComMode);
#endif /* if 0 begin, no init escpr-lib */

/*     Initialize library (Connect to USB backend) */
	int ret2 = ECB_DAEMON_NO_ERROR;
	ret2 = Init_Lib();
	if (ret2 !=  ECB_DAEMON_NO_ERROR){
	 	printf ("Communication daemon down, Error code = %d\n", ret2 );
		exit(1);
	}


    return true;
}

EPS_ERR_CODE EPSCommunicator::NozzleCheckPrinter()
{
	qDebug()<<"Check call function NozzleCheckPrinter";
	EPS_ERR_CODE        retState;              /* Return state                         */

	if(m_currentPrinter.protocol == EPS_PROTOCOL_USB){
		
/*     Nozzle Check (from USB Backend)*/
		ECB_PRINTER_ERR ret = Nozzle_Check();
		if (ret == ECB_PRNERR_NOERROR){
			retState = EPS_ERR_NONE;
		}else{
			retState = EPS_PRNERR_COMM;
		}

	}else {
		/* do nothing here, communicator using escpr-lib */
	}
/*
		else{

		EPS_JOB_ATTRIB		jobAttr;
		EPS_UINT8			*buf, *p;
		EPS_UINT32			bufsize, sentsize;
		EPS_PRN_DATA		prnData;
		EPS_STATUS          status;
		retState = EPS_ERR_NONE;
		memset(&jobAttr, 0, sizeof(jobAttr));
		memset(&prnData, 0, sizeof(prnData));
		prnData.version       = EPS_PRN_DATA_VER_CUR;
		prnData.chunk.version = EPS_PRN_DATA_VER_CUR;
		buf = p = NULL;
		bufsize = sentsize = 0;

		jobAttr.version = EPS_JOB_ATTRIB_VER_CUR;
		jobAttr.colorPlane = EPS_CP_PRINTCMD;				
		jobAttr.cmdType = EPS_MNT_NOZZLE;								


		retState = epsStartJob(&jobAttr);

		if(retState == EPS_ERR_NONE)
		{
		    retState = epsMakeMainteCmd(EPS_MNT_NOZZLE, NULL, &bufsize);
		}else {
		    qDebug()<<"Can not start job err: "<<retState;
		    retState = epsCancelJob(0);
		    return retState;

		}

		buf = (EPS_UINT8*)malloc(bufsize);
		retState = epsMakeMainteCmd(EPS_MNT_NOZZLE, buf, &bufsize);

		p = buf;
		while(bufsize > 0 && EPS_ERR_NONE == retState){
		    prnData.chunk.data     = p;
		    prnData.chunk.dataSize = bufsize;

		    retState = epsSendData(&prnData);
		    if( EPS_ERR_NONE != retState ){
		        retState = epsContinueJob();
		    }

		    p += prnData.chunk.dataSize;
		    bufsize -= prnData.chunk.dataSize;
		}

		retState = epsEndJob();
	}
*/

    return retState;
}

EPS_ERR_CODE EPSCommunicator::HeadCleanPrinter()
{
	qDebug()<<"Check call function HeadCleanPrinter";
    EPS_ERR_CODE        retState;              /* Return state                         */

	if(m_currentPrinter.protocol == EPS_PROTOCOL_USB){
		
/*     Head Cleaning (from USB Backend)*/
		ECB_PRINTER_ERR ret = Head_Cleaning();
		
		if (ret == ECB_PRNERR_NOERROR){
			retState = EPS_ERR_NONE;
		}else{
			retState = EPS_PRNERR_COMM;
		}

	}else {
		/* do noting here, escpr-lib func here */
	}
/*
		else{

		EPS_JOB_ATTRIB		jobAttr;
		EPS_UINT8			*buf, *p;
		EPS_UINT32			bufsize, sentsize;
		EPS_PRN_DATA		prnData;
		EPS_STATUS          status;
		retState = EPS_ERR_NONE;
		memset(&jobAttr, 0, sizeof(jobAttr));
		memset(&prnData, 0, sizeof(prnData));
		prnData.version       = EPS_PRN_DATA_VER_CUR;
		prnData.chunk.version = EPS_PRN_DATA_VER_CUR;
		buf = p = NULL;
		bufsize = sentsize = 0;

		jobAttr.version = EPS_JOB_ATTRIB_VER_CUR;			
		jobAttr.colorPlane = EPS_CP_PRINTCMD;				
		jobAttr.cmdType = EPS_MNT_CLEANING;					

		retState = epsStartJob(&jobAttr);

		if(retState == EPS_ERR_NONE)
		{
		    retState = epsMakeMainteCmd(EPS_MNT_CLEANING, NULL, &bufsize);
		}else {
		    qDebug()<<"Can not start job err: "<<retState;
		    retState = epsCancelJob(0);
		    return retState;
		}

		buf = (EPS_UINT8*)malloc(bufsize);
		retState = epsMakeMainteCmd(EPS_MNT_CLEANING, buf, &bufsize);

		p = buf;
		while(bufsize > 0 && EPS_ERR_NONE == retState){
		    prnData.chunk.data     = p;
		    prnData.chunk.dataSize = bufsize;

		    retState = epsSendData(&prnData);
		    if( EPS_ERR_NONE != retState ){
		        retState = epsContinueJob();
		    }

		    p += prnData.chunk.dataSize;
		    bufsize -= prnData.chunk.dataSize;
		}

		retState = epsEndJob();
	}
*/

    return retState;
}

void EPSCommunicator::internalRunFindPrinterLoop()
{
	/* find printer by escpr-lib, do noting here, jump to use usb backend */
/*
    //debug_msg("Start call Function EPSCommunicator::internalRunFindPrinterLoop \n");
    //m_searchAttr.timeout = timeout;
    m_searchAttr.retState = EPS_ERR_NONE;
    m_searchAttr.searchFinished = FALSE;

    //while (m_bKeepFindPrinterThread)
    {
        EPS_ERR_CODE err = epsFindPrinter(m_searchAttr.probe.dsc.addr.protocol, 1*1000);
        if (EPS_ERR_NONE == err)
        {
            qDebug()<<"Did start find printer";
        }
        else
        {
            qDebug()<<"Cann't start find printer: errorCode: "<<err;
        }
        //sleep(15);
    }
    //debug_msg("Exit call Function EPSCommunicator::internalRunFindPrinterLoop \n");
*/
}
/*
EThreadReturnValue EPSCommunicator::RunFindPrinterThread(void *object, EThreadParam param)
{
    EPSCommunicator* communicator = (EPSCommunicator*)object;
    communicator->internalRunFindPrinterLoop();
    return 0;
}
*/
void EPSCommunicator::StartFindPrinter()
{

/* --------------------------------------------------------------------------------*/
/*     Get Model Name (from Network)*/
    qDebug()<<"Start find printer";
    m_printerList.clear();
    memset(&m_currentPrinter, 0x00, sizeof(m_currentPrinter));
    internalRunFindPrinterLoop();

/* --------------------------------------------------------------------------------*/
/*     Get Model Name (from USB Backend)*/
	ECB_PRINTER printer_usb;
	EPS_PRINTER printer;
	char modelname_with_USB[256];
	int rsize = Get_Printer_Info(&printer_usb);
	if(rsize > 0){
		strcpy(printer.manufacturerName , printer_usb.manufacture);
		sprintf(modelname_with_USB, "%s (USB)", printer_usb.modelname);
		strcpy(printer.modelName , modelname_with_USB);
		printer.language = printer_usb.language;
		printer.protocol = EPS_PROTOCOL_USB;

		m_printerList.push_back(printer);

    }else{
		printf("No USB printer\n");
	}
/* --------------------------------------------------------------------------------*/

}

EPS_INT32 EPSCommunicator::didFindPrinter(EPS_PRINTER printer)
{
    qDebug()<<"Did find printer: "<<printer.modelName;
    bool IsPushBackPrinter = true;
    QString PrinterID = QString((char*)printer.printerID);
    QString PrinterName = QString((char*)printer.modelName);
    QVector<EPS_PRINTER> prList = m_printerList;

    for(int i = 0; i < prList.size(); i++)
    {
        QString PrinterIDList = QString((char*)prList[i].printerID);
        QString PrinterNameList = QString((char*)prList[i].modelName);

        if(PrinterID == PrinterIDList && PrinterName == PrinterNameList)
        {
            IsPushBackPrinter = false;
            break;
        }

    }

    if(IsPushBackPrinter)
    {
		
        m_printerList.push_back(printer);
    }
    return EPS_ERR_NONE;
}



EPS_INT32 EPSCommunicator::FindPrinterCallback(EPS_PRINTER printer)
{
    if (pSelf)
    {
        return pSelf->didFindPrinter(printer);
    }
    return EPS_ERR_INVALID_CALL;
}

bool EPSCommunicator::ConnectToPrinter(const QString &printerName, const QString &printerId)
{

    EPS_ERR_CODE code = EPS_ERR_NONE;
    //QString currentPrinterId = QString((char*)m_currentPrinter.printerID);
    QString currentPrinterName = QString((char*)m_currentPrinter.modelName);

    qDebug()<<"printerName ======"<<printerName;
    qDebug()<<"printerId ======"<<printerId;

    QVector<EPS_PRINTER> prList = m_printerList;
    if(currentPrinterName != printerName)
    {
        int i = 0;
        for(i = 0; i< prList.size(); i++)
        {
            qDebug()<<"prList[i].printerID ==== "<<prList[i].printerID;
            qDebug()<<"prList[i].modelName ==== "<<prList[i].modelName;

            if((prList[i].printerID == printerId) && prList[i].modelName == printerName)
            {
                m_currentPrinter = prList[i];
                break;
            }
        }
		/* todo: no use escpr-lib api here */
        //code = epsSetPrinter(&m_currentPrinter);
        //qDebug()<<"epcomSetPrinter code: "<<code;
    }

    //return (code == EPS_ERR_NONE);
    return true;
}

bool EPSCommunicator::GetPrinterStatus(EPS_STATUS* status)
{
    /*const EPS_UINT8* buffer = NULL;
    EPS_INT32 bufferSize = 0;

    EPS_ERR_CODE ret = epcomGetInformation(EPCOM_INF_STATUS, &buffer, &bufferSize);
    if(ret == EPS_ERR_NONE)
    {
        EPSReplyParser* parser = EPSReplyParser::GetInstance();
        status->size = sizeof(EPRP_STATUS);
        if(parser->ParseStatus(status, buffer, bufferSize))
        {
             qDebug()<<"Parse status ok";

        }else{
            qDebug()<<"Parse NG!";
            return false;
        }
        return true;
    }*/
    return true;
}


bool EPSCommunicator::GetCurrentPrinterStatus(EPS_STATUS* status, EPS_INK_INFO* inkInfo)
{
    EPS_ERR_CODE retState = EPS_ERR_NONE;
	ECB_PRINTER_STS retState_usb = ECB_PRNST_IDLE;

	if(m_currentPrinter.protocol == EPS_PROTOCOL_USB){
 
/*     Get Status (from USB Backend)*/
		ECB_STATUS status_usb;

		retState_usb =Get_Status(&status_usb);

		if(retState_usb == ECB_DAEMON_NO_ERROR){
			status->printerStatus = status_usb.printerStatus;

			status->printerStatus = status_usb.printerStatus;
			status->errorCode = status_usb.errorCode;
			status->jobContinue = false;

			inkInfo->number= status_usb.ink_num;

			int i;
			for(i=0; i < status_usb.ink_num; i++){
					inkInfo->colors[i]=status_usb.colors[i];
					inkInfo->remaining[i]=status_usb.inklevel[i];
					inkInfo->status[i]=EPS_INK_ST_NORMAL; //not used in this STM
			}

			inkInfo->showInkInfo=status_usb.showInkInfo;
			inkInfo->showInkLow=status_usb.showInkLow;

		    status->paper_count.color = status_usb.paper_count.color;
		    status->paper_count.monochrome = status_usb.paper_count.monochrome;			
		    status->paper_count.blank = status_usb.paper_count.blank;		
		    status->paper_count.adf = status_usb.paper_count.adf;
		    status->paper_count.color_borderless = status_usb.paper_count.color_borderless;		
		    status->paper_count.monochrome_borderless = status_usb.paper_count.monochrome_borderless;	

			return true;
		}else{
			qDebug()<<"can't get current status for usb backend.";
			return false;
		}
}
