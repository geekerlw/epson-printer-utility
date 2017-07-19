#include "EPUView.h"
#include "EPUPrinterController.h"

EPUView::EPUView(QWidget *parent)
	 : QWidget(parent)
{
    m_pDelegate = NULL;
    m_pMainLayout = new QVBoxLayout(this);
    m_pMainLayout->setMargin(5);
    m_pMainLayout->setSpacing(0);
    m_pMainLayout->setAlignment(Qt::AlignTop);
    
    m_pPrinterNameLayout = new QHBoxLayout();
    m_pPrinterNameLayout->setMargin(10);
    m_pPrinterNameLayout->setSpacing(10);
    m_pPrinterNameLayout->setAlignment(Qt::AlignLeft);
    
    m_pMainLayout->addLayout(m_pPrinterNameLayout);
    
    m_pEpsonLogo = new QLabel(tr(""));
    QPixmap pix(":/Images/epson.png");
    m_pEpsonLogo->setPixmap(pix);
    m_pEpsonLogo->setFixedSize(QSize(120, 30));
    m_pPrinterNameLayout->addWidget(m_pEpsonLogo);

    m_pPrinterNameLabel = new QLabel(tr("EPSON Epson ME OFFICE 960FWD"));
    m_pPrinterNameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pPrinterNameLabel->setFixedHeight(30);
    QFont font;
    font.setFamily(font.defaultFamily());
    font.setBold(true);
    font.setPointSize(15);
    m_pPrinterNameLabel->setFont(font);
    m_pPrinterNameLabel->setAlignment(Qt::AlignVCenter);
    m_pPrinterNameLayout->addWidget(m_pPrinterNameLabel);
}


void EPUView::UpdateCurrentPrinter()
{
    EPUPrinter* currentPrinter = EPUPrinterController::GetInstance()->GetCurrentPrinter();
    if (currentPrinter)
    {
        m_pPrinterNameLabel->setText(currentPrinter->GetPrinterName());
    }
    else
    {
        qDebug()<<"Can not get current Printer Name";
        m_pPrinterNameLabel->setText("");
    }
}
