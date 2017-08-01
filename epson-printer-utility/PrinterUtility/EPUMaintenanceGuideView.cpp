#include "EPUMaintenanceGuideView.h"
#include "../EPSCommonLib/EPSPrinterController.h"
#include "EPUMainWindow.h"

EPUMaintenanceGuideView::EPUMaintenanceGuideView(QWidget *parent)
    : EPUOperationView(parent)
    , m_nMaintenanceCommand(EPU_MAINTENANCE_COMMAND_NONE)
{
    m_pMaintenanceGuideLabel = new QLabel;
    m_pMaintenanceGuideLabel->setWordWrap(true);
    m_pMaintenanceGuideLabel->setAlignment(Qt::AlignLeft);
    m_pMaintenanceGuideLabel->setObjectName("MaintenanceGuideLabel");
    m_pMaintenanceGuideLabel->setStyleSheet(
                "#MaintenanceGuideLabel {"
                "border-radius: 3px;"
                "background-color: rgba(255, 255, 255, 255);"
                "}");
    m_pMaintenanceGuideLabel->setFixedHeight(220);
    m_pMaintenanceGuideLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHBoxLayout* temp = new QHBoxLayout;
    temp->setMargin(20);
    temp->addWidget(m_pMaintenanceGuideLabel);
    m_pGuideLayout->addLayout(temp);

    m_pPrintButtonLayout = new QHBoxLayout;
    m_pPrintButtonLayout->setMargin(20);
    m_pPrintButtonLayout->setSpacing(10);
    m_pPrintButtonLayout->setAlignment(Qt::AlignRight);
    m_pCancelButton = new QPushButton(QLabel::tr("Cancel"));
    m_pCancelButton->setFixedSize(100, 30);
    m_pPrintButtonLayout->addWidget(m_pCancelButton);
    m_pPrintButton = new QPushButton(QLabel::tr("Print"));
    m_pPrintButton->setFixedSize(100, 30);
    m_pPrintButtonLayout->addWidget(m_pPrintButton);
    m_pGuideLayout->addLayout(m_pPrintButtonLayout);

    setupConnection();
}

void EPUMaintenanceGuideView::SetMaintenanceCommand(EPUMaintenanceCommand command)
{
    m_nMaintenanceCommand = command;
    qDebug()<<"Maintenacen command: "<<command;

    if (command == EPU_MAINTENANCE_COMMAND_NOZZLE_CHECK)
    {
        m_pPrintButton->setText(QLabel::tr("Print"));
        QPixmap pix(":/Images/Nozzle_small_old.png");
        m_pGuideIconView->setPixmap(pix);
        m_pGuideLabel->setText(QLabel::tr("Nozzle Check"));

        m_pMaintenanceGuideLabel->setText(QLabel::tr("To perform a nozzle check, make sure that the printer is on and connected to the computer."));
    }
    else if (command == EPU_MAINTENANCE_COMMAND_HEAD_CLEANING)
    {
        m_pPrintButton->setText(QLabel::tr("Start"));
        QPixmap pix(":/Images/Cleaning_small_old.png");
        m_pGuideIconView->setPixmap(pix);
        m_pGuideLabel->setText(QLabel::tr("Head Cleaning"));
        m_pMaintenanceGuideLabel->setText(QLabel::tr("Click [Start] to clean the print head nozzles. Because cleaning consumes ink, only clean the print head when faint areas or gaps appear in your printout. Use the \"Nozzle Check\" utility first to confirm that the print head needs to be cleaned."));
    }
}

void EPUMaintenanceGuideView::setupConnection()
{
    QObject::connect(m_pCancelButton, SIGNAL(clicked()), this, SLOT(btnCancelClicked()));
    QObject::connect(m_pPrintButton, SIGNAL(clicked()), this, SLOT(btnPrintClicked()));
}

void EPUMaintenanceGuideView::btnCancelClicked()
{
    if (m_pDelegate)
    {
        m_pDelegate->EPUViewDidCloseWithCode(this, EPU_CALLBACK_FINISH);
    }
}

void EPUMaintenanceGuideView::btnPrintClicked()
{
    QString printerName;
    QString printerID;
    EPSPrinterController* controlLib = EPSPrinterController::GetInstance();
    EPUPrinterController* controller = EPUPrinterController::GetInstance();

    printerName = controller->GetCurrentPrinter()->GetModelName();
    printerID = controller->GetCurrentPrinter()->GetPrinterId();

    if(m_nMaintenanceCommand == EPU_MAINTENANCE_COMMAND_NOZZLE_CHECK)
    {
        if(controlLib->ConnectToPrinter(printerName, printerID))
        {
            controlLib->NozzleCheckPrinter();
        }
    }else if(m_nMaintenanceCommand == EPU_MAINTENANCE_COMMAND_HEAD_CLEANING)
    {
        if(controlLib->ConnectToPrinter(printerName, printerID))
        {
            controlLib->HeadCleanPrinter();
        }
    }

    if (m_pDelegate)
    {
        m_pDelegate->EPUViewDidCloseWithCode(this, EPU_CALLBACK_FINISH);
    }
}



