#include "EPUMainView.h"
#include "EPUPrinterController.h"
#include "EPUMainWindow.h"
#include "../EPSCommonLib/EPSPrinterController.h"
#include <QTimer>

EPUMainView::EPUMainView(QWidget *parent)
    : EPUView(parent)
    , m_pPrinterListDlg(NULL)
{

    m_pPrinterStatusBox = new QGroupBox;
    m_pPrinterStatusBox->setFlat(false);
    m_pPrinterStatusBox->setObjectName("PrinterStatusBox");
    m_pPrinterStatusBox->setStyleSheet(
                "#PrinterStatusBox {"
                "border-radius: 5px;"
                "background-color: rgba(221, 221, 221, 255);"
                "}");

    QHBoxLayout* statusBoxLayout = new QHBoxLayout;
    statusBoxLayout->setMargin(10);
    statusBoxLayout->setSpacing(20);
    statusBoxLayout->addSpacing(10);
    statusBoxLayout->addWidget(m_pPrinterStatusBox);
    statusBoxLayout->addSpacing(10);
    m_pMainLayout->addLayout(statusBoxLayout);

    m_pPrinterStatusLayout = new QHBoxLayout;
    m_pPrinterStatusLayout->setMargin(0);
    m_pPrinterStatusLayout->setSpacing(0);
    m_pPrinterStatusLayout->setAlignment(Qt::AlignLeft);
    m_pPrinterStatusLayout->addSpacing(5);
    m_pPrinterStatusBox->setLayout(m_pPrinterStatusLayout);

    m_pPrinterLogo = new QLabel;
    m_pPrinterLogo->setFixedSize(QSize(90, 70));

    m_pPrinterLogo->setObjectName("PrinterLogo");
    m_pPrinterLogo->setStyleSheet(
                "#PrinterLogo {"
                "border: none;"
                "outline: none;"
                "border-radius: 3px;"
                "background-color: rgba(0, 255, 255, 0);"
                "color: #000000;"
                "background-image: url(:/Images/printer_status.png);"
                "background-position: left;"
                "background-repeat: none;"
                "text-align: bottom;"
                "font-size: 11pt;"
                "padding-right: 0px;"
                "padding-left: 30px;"
                "padding-bottom: 0px;"
                "}"
                );

    m_pPrinterStatusLayout->addWidget(m_pPrinterLogo);

    m_pUpdateStatusLayout = new QVBoxLayout;
    m_pUpdateStatusLayout->setSpacing(5);
    m_pUpdateStatusLayout->setMargin(5);
    m_pUpdateStatusLayout->setAlignment(Qt::AlignTop);
    m_pPrinterStatusLayout->addLayout(m_pUpdateStatusLayout);
    m_pPrinterStatusLayout->addSpacing(10);

    m_pPrinterStateLabel = new QLabel(tr("Disconnected"));
#if 0
    m_pPrinterStateLabel->setAlignment(Qt::AlignVCenter);
    m_pPrinterStateLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pPrinterStateLabel->setFixedHeight(22);
    m_pPrinterStateLabel->setObjectName("PrinterStateLabel");
    m_pPrinterStateLabel->setStyleSheet(
                "#PrinterStateLabel {"
                "border: none;"
                "outline: none;"
                "border-radius: 3px;"
                "background-color: rgba(255, 255, 255, 0);"
                "color: #000000;"
                "background-image: url(:/Images/status_indicator_offline.png);"
                "background-position: left;"
                "background-repeat: none;"
                "text-align: bottom;"
                "font-size: 11pt;"
                "padding-right: 0px;"
                "padding-left: 30px;"
                "padding-bottom: 0px;"
                "}"
                );
    m_pUpdateStatusLayout->addWidget(m_pPrinterStateLabel);
#endif
    m_pPrinterStatusLabel = new QLabel(tr("Printer disconnected"));
    m_pPrinterStatusLabel->setAlignment(Qt::AlignLeft);
    m_pPrinterStatusLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pPrinterStatusLabel->setFixedHeight(60);
    m_pPrinterStatusLabel->setObjectName("PrinterStatusLabel");
    m_pPrinterStatusLabel->setStyleSheet(
                "#PrinterStatusLabel {"
                "border-style: outset;"
                "border-width: 1px;"
                "border-color: black;"
                "outline: none;"
                "border-radius: 3px;"
                "background-color: rgba(255, 255, 255, 255);"
                "color: #000000;"
                "text-align: bottom;"
                "font-size: 11pt;"
                "padding-right: 0px;"
                "padding-left: 0px;"
                "padding-bottom: 0px;"
                "}"
                );
    m_pUpdateStatusLayout->addWidget(m_pPrinterStatusLabel);

    QHBoxLayout* inkTempLayout = new QHBoxLayout;
    inkTempLayout->setSpacing(0);
    inkTempLayout->setMargin(0);

    m_pMainLayout->addLayout(inkTempLayout);

//    m_pInkLevelsGroup = new QGroupBox(QLabel::tr("Ink Levels"));
    m_pInkLevelsGroup = new QGroupBox(QLabel::tr(""));
    m_pInkLevelsGroup->setFlat(false);
    m_pInkLevelsGroup->setObjectName("InkLevelsGroupBox");
    m_pInkLevelsGroup->setStyleSheet(
                "#InkLevelsGroupBox {"
                "border-radius: 5px;"
                "background-color: rgba(221, 221, 221, 255);"
                "}");

    m_pInkLevelsGroup->setFixedHeight(180);

    m_pInkViewLayout = new QVBoxLayout;
    m_pInkViewLayout->setMargin(10);
    m_pInkViewLayout->setSpacing(0);

    m_pInkLevelsGroup->setLayout(m_pInkViewLayout);
    m_pInkBox = new QHBoxLayout;
    m_pInkBox->setAlignment(Qt::AlignHCenter);
    m_pInkViewLayout->addLayout(m_pInkBox);

/*
    //set default ink
    EPUInkLevelView* ink = new EPUInkLevelView(Qt::black, "");
    ink->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ink->setFixedHeight(200);
    m_inkLevelViewArray.push_back(ink);
    m_pInkBox->addWidget(ink);
    ink->SetInkLevel(0);

    ink = new EPUInkLevelView(Qt::magenta, "");
    ink->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ink->setFixedHeight(200);
    m_inkLevelViewArray.push_back(ink);
    m_pInkBox->addWidget(ink);
    ink->SetInkLevel(0);

    ink = new EPUInkLevelView(Qt::yellow, "");
    ink->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ink->setFixedHeight(200);
    m_inkLevelViewArray.push_back(ink);
    m_pInkBox->addWidget(ink);
    ink->SetInkLevel(0);

    ink = new EPUInkLevelView(Qt::cyan, "");
    ink->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ink->setFixedHeight(200);
    m_inkLevelViewArray.push_back(ink);
    m_pInkBox->addWidget(ink);
    ink->SetInkLevel(0);
*/
    inkTempLayout->addSpacing(20);
    inkTempLayout->addWidget(m_pInkLevelsGroup);
    inkTempLayout->addSpacing(20);


// Parent Field of Papper Counter Label and Change Printer Button
    QHBoxLayout* printerListLayout = new QHBoxLayout;

    printerListLayout->setMargin(20);
    printerListLayout->setSpacing(0);
    printerListLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);


// Paper Counter Label
    m_pPaperCountLabel = new QLabel(this);
    QPalette pal2 = m_pPaperCountLabel->palette();
    pal2.setColor(QPalette::Foreground, QColor("#808080"));
    m_pPaperCountLabel->setPalette(pal2);
    m_pPaperCountLabel->setText(QString::fromUtf8(""));   //Initialized by blank string.
    m_pPaperCountLabel->setFixedSize(475, 30);

	QFont font;
	font.setPointSize(10);
	font.setBold(true);
	
	m_pPaperCountLabel->setFont(font);
//	m_pPaperCountLabel->setAutoFillBackground(true);

    printerListLayout->addWidget(m_pPaperCountLabel);

// Change Printer Button
    m_pPrinterListButton = new QPushButton(this);
	m_pPrinterListButton->setText(QLabel::tr("Printer Change"));
//    m_pPrinterListButton->setFixedSize(110, 30);
    printerListLayout->addWidget(m_pPrinterListButton);

    m_pMainLayout->addLayout(printerListLayout);

    m_timer = new QTimer(this);
    connection();
    setupConnection();

}

EPUMainView::~EPUMainView () {
    m_timer->stop();
}

void EPUMainView::connection()
{
    connect(m_timer, SIGNAL(timeout()),this, SLOT(update()));
    m_timer->start(2000); //
}

void EPUMainView::update()
{
    EPUPrinterController* controller = EPUPrinterController::GetInstance();
    controller->StartUpdatePrinterStatus();
}

void EPUMainView::updatePrinterStatus()
{
    EPUPrinterController* controller = EPUPrinterController::GetInstance();
    EPUPrinter* printer = controller->GetCurrentPrinter();

    if(printer->GetPrinterDisconnected())
    {
        updateCurrenPrinterDisconnected(false);
        if (m_pDelegate)
        {
            m_pDelegate->EPUViewDisAble();
        }
    }else
    {
        updateCurrenPrinterStaus();
    }
}


void EPUMainView::setupConnection()
{
    QObject::connect(m_pPrinterListButton, SIGNAL(clicked()), this, SLOT(btnShowPrinterListClicked()));
}

void EPUMainView::updatePrinterInkInfo()
{

    EPS_INK_INFO inkInfo;
    EPS_STATUS status;

    EPUPrinterController* controller = EPUPrinterController::GetInstance();
    status = controller->GetCurrentPrinter()->GetPrinterStatus();

    if(/*status.printerStatus == 4 && */status.errorCode == 103 || status.errorCode == 104){
        for (int i = 0; i < m_inkLevelViewArray.size(); i++)
        {
            EPUInkLevelView* ink = m_inkLevelViewArray[i];
            QColor color = Qt::black;
            QString name = "";
            int level = 0;
            ink->SetInkInfo(color, name, level);
        }
        return;
    }
    inkInfo = controller->GetCurrentPrinter()->GetInkInfo();


//   if (inkInfo.number != m_inkLevelViewArray.size())
	if(1)
    {

        for (int i = 0; i < m_inkLevelViewArray.size(); i++)
        {
            EPUInkLevelView* ink = m_inkLevelViewArray[i];
            m_pInkBox->removeWidget(ink);
            delete ink;
        }
        m_inkLevelViewArray.clear();

		if  (inkInfo.showInkInfo){//
				for (int i = 0; i < inkInfo.number; i++)
				{
				    EPUInkLevelView* ink = new EPUInkLevelView(Qt::black);
				    ink->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
				    ink->setFixedHeight(200);
				    ink->SetInkLevel(0);
					ink->SetSmartCharge(FALSE);
				    m_inkLevelViewArray.push_back(ink);
				    m_pInkBox->addWidget(ink);
				}

				for (int i = 0; i < inkInfo.number; i++)
				{
				    QColor color = eprpColorToQColor(inkInfo.colors[i]);
				    QString name = eprpColorToQStringName(inkInfo.colors[i]);
				    int level = inkInfo.remaining[i];
				    EPUInkLevelView* ink = m_inkLevelViewArray[i];
				    ink->SetInkColor(color);
//				    ink->SetInkName(name);
				    qDebug()<<"Add new ink: "<<name <<" color: "<<color<<" level: "<<level;
				    ink->SetInkLevel(level);
				}

		}else{//
				//Ink Low エラーのチェック
				QString EndInk[EPS_INK_NUM];
				int endink_num = 0;

				for (int i = 0; i < inkInfo.number; i++)
				{
				    QColor color = eprpColorToQColor(inkInfo.colors[i]);
				    QString name = eprpColorToQStringName(inkInfo.colors[i]);
				    int status = inkInfo.status[i];
					
//					status = EPS_INK_ST_END; //for Debug

//					if(level == 0){
					if(status == EPS_INK_ST_END){
						EndInk[endink_num++] = name;
//					    qDebug()<<"EndInk: "<<name;
					}

				}

				if(endink_num > 0 ){
					PrinterLogoError();
					setPrinterStatus(QLabel::tr("Replacement ink supply unit"));
				}

//				qDebug()<<"EndInkNum: "<<endink_num;

			    EPUInkLevelView* ink = new EPUInkLevelView(Qt::black);
			    ink->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
			    ink->setFixedHeight(100);
				ink->SetSmartCharge(TRUE);//
				ink->SetInkEndCartridge(endink_num, EndInk);
			    m_inkLevelViewArray.push_back(ink);
			    m_pInkBox->addWidget(ink);
		}


    }
    else
    {


#if 0
        for (int i = 0; i < inkInfo.number; i++)
        {
            EPUInkLevelView* ink = m_inkLevelViewArray[i];
            QColor color = eprpColorToQColor(inkInfo.colors[i]);
            QString name = eprpColorToQStringName(inkInfo.colors[i]);
            int level = inkInfo.remaining[i];
            qDebug()<<"Update ink: "<<name <<" color: "<<color<<" level: "<<level;
            ink->SetInkInfo(color, name, level);
            ink->setFixedHeight(200);
        }
#endif

}



}


void EPUMainView::PrinterLogoError()
{
m_pPrinterLogo->setStyleSheet(
            "#PrinterLogo {"
            "border: none;"
            "outline: none;"
            "border-radius: 3px;"
            "background-color: rgba(0, 255, 255, 0);"
            "color: #000000;"
            "background-image: url(:/Images/printer_status_error.png);"
            "background-position: left;"
            "background-repeat: none;"
            "text-align: bottom;"
            "font-size: 11pt;"
            "padding-right: 0px;"
            "padding-left: 30px;"
            "padding-bottom: 0px;"
            "}"
            );
}

void EPUMainView::PrinterLogoErrorLaser()
{
m_pPrinterLogo->setStyleSheet(
            "#PrinterLogo {"
            "border: none;"
            "outline: none;"
            "border-radius: 3px;"
            "background-color: rgba(0, 255, 255, 0);"
            "color: #000000;"
            "background-image: url(:/Images/printer_status_error_laser.png);"
            "background-position: left;"
            "background-repeat: none;"
            "text-align: bottom;"
            "font-size: 11pt;"
            "padding-right: 0px;"
            "padding-left: 30px;"
            "padding-bottom: 0px;"
            "}"
            );
}


void EPUMainView::PrinterLogoWarning()
{
m_pPrinterLogo->setStyleSheet(
            "#PrinterLogo {"
            "border: none;"
            "outline: none;"
            "border-radius: 5px;"
            /*"background-color: rgb(255, 137, 137);"*/
            "color: #270000;"
            "background-image: url(:/Images/printer_status_warning.png);"
            "background-position: left;"
            "background-repeat: none;"
            "text-align: bottom;"
            "font-size: 11pt;"
            "padding-right: 0px;"
            "padding-left: 60px;"
            "padding-bottom: 0px;"
            "}"
            );
}

void EPUMainView::PrinterLogoWarningLaser()
{
#if 0
m_pPrinterLogo->setStyleSheet(PrinterLogoNormal
            "#PrinterLogo {"
            "border: none;"
            "outline: none;"
            "border-radius: 5px;"
            /*"background-color: rgb(255, 137, 137);"*/
            "color: #270000;"
            "background-image: url(:/Images/printer_status_warning_laser.png);"
            "background-position: left;"
            "background-repeat: none;"
            "text-align: bottom;"
            "font-size: 11pt;"
            "padding-right: 0px;"
            "padding-left: 60px;"
            "padding-bottom: 0px;"
            "}"
            );
#endif
}

void EPUMainView::updateCurrenPrinterDisconnected(bool first)
{
    m_pPrinterStateLabel->setStyleSheet(
                "#PrinterStateLabel {"
                "border: none;"
                "outline: none;"
                "border-radius: 3px;"
                "background-color: rgba(255, 255, 255, 0);"
                "color: #000000;"
                "background-image: url(:/Images/status_indicator_offline.png);"
                "background-position: left;"
                "background-repeat: none;"
                "text-align: bottom;"
                "font-size: 11pt;"
                "padding-right: 0px;"
                "padding-left: 30px;"
                "padding-bottom: 0px;"
                "}"
                );

    PrinterLogoError();

    setPrinterState("Disconnected");

	if(first)
	{
	    setPrinterStatus(QLabel::tr("Please wait..."));
	}else{
	    setPrinterStatus(QLabel::tr("Communication Error"));
	}

    if (m_pDelegate)
    {
        m_pDelegate->EPUViewDisAble();
    }
    updateDefaultInklevel();
}

void EPUMainView::PrinterLogoNormal()
{
    m_pPrinterLogo->setStyleSheet(
                "#PrinterLogo {"
                "border: none;"
                "outline: none;"
                "border-radius: 3px;"
                "background-color: rgba(255, 255, 255, 0);"
                "color: #000000;"
                "background-image: url(:/Images/printer_status.png);"
                "background-position: left;"
                "background-repeat: none;"
                "text-align: bottom;"
                "font-size: 11pt;"
                "padding-right: 0px;"
                "padding-left: 30px;"
                "padding-bottom: 0px;"
                "}"
                );
}

void EPUMainView::PrinterLogoNormalLaser()
{
    m_pPrinterLogo->setStyleSheet(
                "#PrinterLogo {"
                "border: none;"
                "outline: none;"
                "border-radius: 3px;"
                "background-color: rgba(255, 255, 255, 0);"
                "color: #000000;"
                "background-image: url(:/Images/printer_status_laser.png);"
                "background-position: left;"
                "background-repeat: none;"
                "text-align: bottom;"
                "font-size: 11pt;"
                "padding-right: 0px;"
                "padding-left: 30px;"
                "padding-bottom: 0px;"
                "}"
                );

}

void EPUMainView::StatusLabelEmptyPrinterList()
{
    m_pPrinterStateLabel->setStyleSheet(
                "#PrinterStateLabel {"
                "border: none;"
                "outline: none;"
                "border-radius: 3px;"
                "background-color: rgba(255, 255, 255, 0);"
                "color: #000000;"
                "background-image: url(:/Images/status_indicator_offline.png);"
                "background-position: left;"
                "background-repeat: none;"
                "text-align: bottom;"
                "font-size: 11pt;"
                "padding-right: 0px;"
                "padding-left: 30px;"
                "padding-bottom: 0px;"
                "}"
                );
    PrinterLogoError();
    setPrinterState("Disconnected");
    setPrinterStatus(QLabel::tr("The printer was not found"));
    updateDefaultInklevel();
}

void EPUMainView::updatePrinterState(int status, int error)
{
    switch(status)
    {
		case EPS_PRNST_ERROR:
			PrinterLogoError();
			switch(error)
			{
				case EPS_PRNERR_GENERAL:
				case EPS_PRNERR_FATAL:
				    setPrinterStatus(QLabel::tr("Genenal Error"));
				    break;
				case EPS_PRNERR_INTERFACE:
				case EPS_PRNERR_BUSY:
				    setPrinterStatus(QLabel::tr("Printer busy"));
				    break;
				case EPS_PRNERR_COVEROPEN:
				    setPrinterStatus(QLabel::tr("Printer cover error"));
				    break;
				case EPS_PRNERR_PAPERJAM:
				    setPrinterStatus(QLabel::tr("Paper jam"));
				    break;
				case EPS_PRNERR_INKOUT:
				case EPS_PRNERR_SHUTOFF:
				case EPS_PRNERR_PRINTPACKEND:
				    setPrinterStatus(QLabel::tr("Replace Cartridge"));
				    break;
				case EPS_PRNERR_PAPEROUT:
				    setPrinterStatus(QLabel::tr("Paper out or not loaded correctly"));
				    break;
				case EPS_PRNERR_SIZE_TYPE_PATH:
				    setPrinterStatus(QLabel::tr("Paper size and paper type settings do not match the printer settings"));
				    break;
				case EPS_PRNERR_SERVICEREQ:
				    setPrinterStatus(QLabel::tr("Service required"));
				    break;
				case EPS_PRNERR_DOUBLEFEED:
				    setPrinterStatus(QLabel::tr("Page not printed or multi-page error"));
				    break;
				case EPS_PRNERR_INKCOVEROPEN:
				    setPrinterStatus(QLabel::tr("Ink cover open"));
				    break;
				case EPS_PRNERR_NOTRAY:
				    setPrinterStatus(QLabel::tr("Cassette tray is not installed correctly"));
				    break;
				case EPS_PRNERR_CARDLOADING:
				    setPrinterStatus(QLabel::tr("Media not loaded correctly"));
				    break;
				case EPS_PRNERR_CDDVDCONFIG:
				case EPS_PRNERR_CDGUIDECLOSE:
				    setPrinterStatus(QLabel::tr("CD/DVD not set correctly"));
				    break;
				case EPS_PRNERR_CDREXIST_MAINTE:
				case EPS_PRNERR_CDRGUIDEOPEN:
				    setPrinterStatus(QLabel::tr("CD/DVD Tray in Use"));
				    break;
				case EPS_PRNERR_CARTRIDGEOVERFLOW:
				    setPrinterStatus(QLabel::tr("Photo cartridge at end of service life"));
				    break;
				case EPS_PRNERR_BATTERYVOLTAGE:
				    setPrinterStatus(QLabel::tr("Battery error"));
				    break;
				case EPS_PRNERR_BATTERYTEMPERATURE:
				    setPrinterStatus(QLabel::tr("Abnormal battery temperature"));
				    break;
				case EPS_PRNERR_BATTERYEMPTY:
				    setPrinterStatus(QLabel::tr("Battery power is almost out"));
				    break;
				case EPS_PRNERR_NOT_INITIALFILL:
				    setPrinterStatus(QLabel::tr("Initial setup error"));
				    break;
				case EPS_PRNERR_SCANNEROPEN:
				    setPrinterStatus(QLabel::tr("Scanner unit open"));
				    break;
				case EPS_PRNERR_FACTORY:
				case EPS_PRNERR_CEMPTY:
				case EPS_PRNERR_CFAIL:
				    setPrinterStatus(QLabel::tr("Ink cartridges cannot be recognized"));
				    break;
				case EPS_PRNERR_COMM:
				    setPrinterStatus(QLabel::tr("Communication Error"));
				    break;
				case EPS_PRNERR_TRAYCLOSE:
				    setPrinterStatus(QLabel::tr("The output tray is closed"));
				    break;
				case EPS_PRNERR_DISABEL_CLEANING:
				    setPrinterStatus(QLabel::tr("Head Cleaning can not be completed"));
				    break;
				default:
				    setPrinterStatus(QLabel::tr("General error"));
				    break;
			}
			break;

		case EPS_PRNST_BUSY:
 			PrinterLogoError();
			setPrinterStatus(QLabel::tr("Printer busy"));
			break;
		case EPS_PRNST_PRINTING:
			setPrinterStatus(QLabel::tr("Printing"));
			break;
		case EPS_PRNST_IDLE:
			setPrinterStatus(QLabel::tr("Ready"));
			break;
    }
    return;
}

void EPUMainView::updateCurrenLaserPrinterStaus()
{
#if 0
    bool IsPrinterInkOut = false;
    bool IsPrinterInkLow = false;
    EPS_STATUS status;
    EPS_INK_INFO inkInfo;

    EPUPrinterController* controller = EPUPrinterController::GetInstance();
    status = controller->GetCurrentPrinter()->GetPrinterStatus();
    inkInfo = controller->GetCurrentPrinter()->GetInkInfo();

    for (int i = 0; i < inkInfo.number; i++)
    {
        if(inkInfo.remaining[i] == 0)
        {
            IsPrinterInkOut = true;
            break;
        }
    }


    if(!IsPrinterInkOut)
    {
        qDebug()<<"Call checkInkInfo = 3 0001";
        for (int i = 0; i < inkInfo.number; i++)
        {
            if(inkInfo.remaining[i] > 0 && inkInfo.remaining[i] <= INKLOW)
            {
                IsPrinterInkLow = true;
                break;
            }
        }
    }

    if(IsPrinterInkLow && status.errorCode == EPS_PRNERR_NOERROR)
    {
        if(m_pDelegate)
        {
            m_pDelegate->EPUViewEnAble();
        }
        setPrinterStatus("Ink Low");
        PrinterLogoWarningLaser();
    }else
    {
        updatePrinterState(status.printerStatus, status.errorCode);
    }

    if(!IsPrinterInkLow && status.errorCode == EPS_PRNERR_NOERROR)
    {
        if(m_pDelegate)
        {
            m_pDelegate->EPUViewEnAble();
        }
        PrinterLogoNormalLaser();
    }

    if(status.errorCode != EPS_PRNERR_NOERROR)
    {
        if(m_pDelegate)
        {
            m_pDelegate->EPUViewDisAble();
        }
        PrinterLogoErrorLaser();
    }

    if(IsPrinterInkLow && status.errorCode == EPS_PRNERR_NOERROR)
    {
        for(int i = 0; i < inkInfo.number; i++)
        {
            if(inkInfo.remaining[i] > 0 && inkInfo.remaining[i] <= INKLOW)
            {
                if(m_pDelegate)
                {
                    m_pDelegate->EPUViewDisAbleHeadClean();
                    //m_pDelegate->EPUViewAbleNozzleCheck();
                }
            }
        }

    }

	if(IsPrinterInkOut)
    {
         PrinterLogoErrorLaser();
         if(m_pDelegate)
         {
             m_pDelegate->EPUViewDisAble();
         }
         updatePrinterState(status.printerStatus, status.errorCode);

    }
#endif
}

void EPUMainView::updateCurrenInkJetPrinterStaus()
{
    if(m_pDelegate)
    {
        m_pDelegate->EPUViewEnAble();
    }

    bool IsPrinterInkOut = false;
    bool IsPrinterInkLow = false;
    EPS_STATUS status;
    EPS_INK_INFO inkInfo;

    EPUPrinterController* controller = EPUPrinterController::GetInstance();
    status = controller->GetCurrentPrinter()->GetPrinterStatus();
    inkInfo = controller->GetCurrentPrinter()->GetInkInfo();

    for (int i = 0; i < inkInfo.number; i++)
    {
        if(inkInfo.remaining[i] == 0)
        {
            IsPrinterInkOut = true;
            break;
        }
    }

    if(!IsPrinterInkOut)
    {
        qDebug()<<"Call checkInkInfo = 3 0001";
        for (int i = 0; i < inkInfo.number; i++)
        {
            if(inkInfo.remaining[i] > 0 && inkInfo.remaining[i] < INKLOW)
            {
                IsPrinterInkLow = true;
                break;
            }
        }
    }

	PrinterLogoNormal();
    updatePrinterState(status.printerStatus, status.errorCode);

	int total_count = status.paper_count.color + status.paper_count.monochrome + status.paper_count.blank;
	int total_borderless = status.paper_count.color_borderless + status.paper_count.monochrome_borderless;

	//if Borderless printed Count filed don't exist, set 0;
	if (total_borderless < 0) total_borderless = 0;

	int total_border =  total_count - total_borderless;

	char buf[256];
	if(total_count >= 0) {
   		 m_pPaperCountLabel->setText(QLabel::tr("Number of sheets of paper fed :  %1").arg(total_count));
	}else{
		//if printed Count filed don't exist, nothing is shown.
		m_pPaperCountLabel->setText(QLabel::tr(""));
	}



    if(!IsPrinterInkLow && status.errorCode == EPS_PRNERR_NOERROR)
    {
        if(m_pDelegate)
        {
            m_pDelegate->EPUViewEnAble();
        }
    }else{

		if  (inkInfo.showInkLow){
			PrinterLogoWarning();
		}else{
			PrinterLogoNormal();	//
		}	
	}

    if(status.errorCode != EPS_PRNERR_NOERROR)
    {
        if(m_pDelegate)
        {
            m_pDelegate->EPUViewDisAble();
        }
        PrinterLogoError();
    }

    if(IsPrinterInkLow && status.errorCode == EPS_PRNERR_NOERROR)
    {
        for(int i = 0; i < inkInfo.number; i++)
        {
            if(inkInfo.remaining[i] > 0 && inkInfo.remaining[i] < INKLOW)
            {
                if(m_pDelegate)
                {
                    m_pDelegate->EPUViewDisAbleHeadClean();
                    //m_pDelegate->EPUViewAbleNozzleCheck();
                }
            }
        }
    }

	if(IsPrinterInkOut)
    {
         PrinterLogoError();
         if(m_pDelegate)
         {
             m_pDelegate->EPUViewDisAble();
         }
 //        updatePrinterState(status.printerStatus, status.errorCode);
         updatePrinterState(EPS_PRNST_ERROR, EPS_PRNERR_INKOUT);
    }

}

void EPUMainView::updateCurrenPrinterStaus()
{


    EPS_UINT32 language;

    qDebug()<< __FUNCTION__;
    EPUPrinterController* controller = EPUPrinterController::GetInstance();
	
    if (controller->GetCurrentPrinter())
    {

			if(controller->GetCurrentPrinter()->GetPrinterDisconnected())
			{
				updateCurrenPrinterDisconnected(true);
				return;
			}

        language = controller->GetCurrentPrinter()->GetPrintCmd();
        if(language == EPS_LANG_ESCPR)
        {
            updateCurrenInkJetPrinterStaus();
        }else
        {
            updateCurrenLaserPrinterStaus();
        }
#if 0
        setPrinterState("Connected");
        m_pPrinterStateLabel->setStyleSheet(
                    "#PrinterStateLabel {"
                    "border: none;"
                    "outline: none;"
                    "border-radius: 3px;"
                    "background-color: rgba(255, 255, 255, 0);"
                    "color: #000000;"
                    "background-image: url(:/Images/status_indicator_online.png);"
                    "background-position: left;"
                    "background-repeat: none;"
                    "text-align: bottom;"
                    "font-size: 11pt;"
                    "padding-right: 0px;"
                    "padding-left: 30px;"
                    "padding-bottom: 0px;"
                    "}"
                    );
#endif
        updatePrinterInkInfo();
    }


}

void EPUMainView::btnShowPrinterListClicked()
{
    m_timer->stop();
    qDebug()<<"Even Click Printer List!!!!!";
    if (!m_pPrinterListDlg)
    {
        m_pPrinterListDlg = new EPUPrinterListDialog;
    }
    m_pPrinterListDlg->DoModal();
    if(m_pDelegate)
    {
        m_pDelegate->EPUViewDidCloseWithCode(this, EPU_CALLBACK_FINISH);
    }
    EPUPrinterController* controller = EPUPrinterController::GetInstance();
    if(controller->GetCurrentPrinter())
    {
        if(controller->GetCurrentPrinter()->GetPrinterDisconnected()){
            updateCurrenPrinterDisconnected(true);
        }else if(!controller->GetPrinterList().isEmpty()){
            EPSPrinterController* controlLib = EPSPrinterController::GetInstance();
            controlLib->ConnectToPrinter(controller->GetCurrentPrinter()->GetModelName(),
                                         controller->GetCurrentPrinter()->GetPrinterId());
            qDebug()<<"Check call function Update Printer Status --------------";
            updateCurrenPrinterStaus();
        }
		update();
        m_timer->start(2000);
    }else
    {
        if(m_pDelegate)
        {
            m_pDelegate->EPUViewDisAble();
        }
        StatusLabelEmptyPrinterList();
    }
}


void EPUMainView::updateDefaultInklevel()
{

    for (int i = 0; i < m_inkLevelViewArray.size(); i++)
    {
        EPUInkLevelView* ink = m_inkLevelViewArray[i];
        QColor color = Qt::black;
        QString name = "";
        int level = 0;
        ink->SetInkInfo(color, name, level);
    }

}

QColor EPUMainView::eprpColorToQColor(EPS_INT32 colorCode)
{
    QColor color = Qt::black;
    switch (colorCode)
    {
    case EPS_COLOR_BLACK:
    {
        color = Qt::black;
        break;
    }
    case EPS_COLOR_CYAN:
    {
        color = Qt::cyan;
        break;
    }
    case EPS_COLOR_MAGENTA:
    {
        color = Qt::magenta;
        break;
    }
    case EPS_COLOR_YELLOW:
    {
        color = Qt::yellow;
        break;
    }
    case EPS_COLOR_LIGHTCYAN:
    {
       /*color = Qt::cyan;
        break;*/
        color = QColor::fromRgb(224,255,255,255);
        break;
    }
    case EPS_COLOR_LIGHTMAGENTA:
    {
       /*color = Qt::magenta;
        break;*/
//        color = QColor::fromRgb(255,66,249,255);
        color = QColor::fromRgb(237,83,147,255);
        break;
    }
    case EPS_COLOR_LIGHTYELLOW:
    {
        /*color = Qt::yellow;
        break;*/
        color = QColor::fromRgb(255,255,224,255);
        break;
    }
    case EPS_COLOR_DARKYELLOW:
    {
        color = Qt::darkYellow;
        break;
    }
    case EPS_COLOR_LIGHTBLACK:
    {
        
        /*color = Qt::black;
        break;*/
        color = QColor::fromRgb(105,105,105,255);
        break;
    }
    case EPS_COLOR_RED:
    {
        color = Qt::red;
        break;
    }
    case EPS_COLOR_VIOLET:
    {

        break;
    }
    case EPS_COLOR_MATTEBLACK:
    {
        break;
    }
    case EPS_COLOR_LIGHTLIGHTBLACK:
    {
        break;
    }
    case EPS_COLOR_PHOTOBLACK:
    {
        break;
    }
    case EPS_COLOR_CLEAR:
    {

        break;
    }
    case EPS_COLOR_GRAY:
    {
        color = Qt::gray;
        break;
    }
    case EPS_COLOR_BLACK2:
    {
        break;
    }
    case EPS_COLOR_ORANGE:
    {

        break;
    }
    case EPS_COLOR_GREEN:
    {
        color = Qt::green;
        break;
    }
    case EPS_COLOR_WHITE:
    {
        color = Qt::white;
        break;
    }
    case EPS_COLOR_CLEAN:
    {
        break;
    }
    case EPS_COLOR_UNKNOWN:
    {

    }
    default:
        break;
    }

    return color;
}

QString EPUMainView::eprpColorToQStringName(EPS_INT32 colorCode)
{
    QString color = "Unknown";
    switch (colorCode)
    {
    case EPS_COLOR_BLACK:
    {
        color = QLabel::tr("Black");
        break;
    }
    case EPS_COLOR_CYAN:
    {
        color = QLabel::tr("Cyan");
        break;
    }
    case EPS_COLOR_MAGENTA:
    {
        color = QLabel::tr("Magenta");
        break;
    }
    case EPS_COLOR_YELLOW:
    {
        color = QLabel::tr("Yellow");
        break;
    }
    case EPS_COLOR_LIGHTCYAN:
    {
        color = QLabel::tr("Light Cyan");
        break;
    }
    case EPS_COLOR_LIGHTMAGENTA:
    {
        color = QLabel::tr("Light Magenta");
        break;
    }
    case EPS_COLOR_LIGHTYELLOW:
    {
        color = QLabel::tr("Light Yellow");
        break;
    }
    case EPS_COLOR_DARKYELLOW:
    {
        color = QLabel::tr("Dark Yellow");
        break;
    }
    case EPS_COLOR_LIGHTBLACK:
    case EPS_COLOR_GRAY:
    {
        color = QLabel::tr("Light Black");
        break;
    }
    case EPS_COLOR_RED:
    {
        color = QLabel::tr("Red");
        break;
    }
    case EPS_COLOR_BLACK2:
    {
        color = QLabel::tr("Black2");
        break;
    }
    case EPS_COLOR_MATTEBLACK:
    {
        color = QLabel::tr("Matte Black");
        break;
    }
    case EPS_COLOR_PHOTOBLACK:
    {
        color = QLabel::tr("Photo Black");
        break;
    }
    case EPS_COLOR_LIGHTLIGHTBLACK:
    {
        color = QLabel::tr("Light Gray");
        break;
    }
    case EPS_COLOR_ORANGE:
    {
        color = QLabel::tr("Orange");
        break;
    }
    case EPS_COLOR_GREEN:
    {
        color = QLabel::tr("Green");
        break;
    }
    case EPS_COLOR_WHITE:
    {
        color = QLabel::tr("White");
        break;
    }
    case EPS_COLOR_CLEAN:
    {
        color = QLabel::tr("Clean");
        break;
    }
    case EPS_COLOR_UNKNOWN:
    {

    }
    default:
        break;
    }

    return color;
}
