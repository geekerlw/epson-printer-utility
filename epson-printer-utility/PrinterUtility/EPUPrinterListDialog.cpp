#include "EPUPrinterListDialog.h"
#include "EPUPrinterController.h"
#include "EPUPrinter.h"

EPUPrinterListDialog::EPUPrinterListDialog(QWidget *parent)
    : QDialog(parent)
    , m_bKeepRefreshThreadUpdatePrListRunning(true)
    , m_bRefreshThreadUpdatePrListStopped(true)
    , m_pRefreshPrinterListThread(NULL)
{
    m_pMainLayout = new QVBoxLayout;
    m_pMainLayout->setMargin(10);
    m_pMainLayout->setAlignment(Qt::AlignTop);

    m_pListView = new QListView;
    m_pSourceModel = new QStandardItemModel;
    m_pListView->setModel(m_pSourceModel);
    m_pListView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pListView->setFixedHeight(340);
    m_pListView->setSelectionMode(QAbstractItemView::SingleSelection);

    m_pSelectionModel = new QItemSelectionModel(m_pSourceModel);
    m_pListView->setSelectionModel(m_pSelectionModel);

    m_pMainLayout->addWidget(m_pListView);

    m_pButtonLayout = new QHBoxLayout;
    m_pButtonLayout->setAlignment(Qt::AlignRight);

    /*m_pUpdateButton = new QPushButton(tr("Update"));
    m_pUpdateButton->setFixedSize(100, 30);
    m_pButtonLayout->addWidget(m_pUpdateButton);*/

    m_pOKButton = new QPushButton(tr("OK"));
    m_pOKButton->setFixedSize(100, 30);
    m_pButtonLayout->addWidget(m_pOKButton);

    m_pMainLayout->addLayout(m_pButtonLayout);

    setLayout(m_pMainLayout);
    setFixedSize(300, 400);
    setWindowTitle(tr("Printer List"));

    //QObject::connect(m_pUpdateButton, SIGNAL(clicked()), this, SLOT(btnUpdateClicked()));
    QObject::connect(m_pOKButton, SIGNAL(clicked()), this, SLOT(btnOKClicked()));
}

EPUPrinterListDialog::~EPUPrinterListDialog()
{
    m_bKeepRefreshThreadUpdatePrListRunning = false;
    if (m_pRefreshPrinterListThread)
    {
        EThreadFree(m_pRefreshPrinterListThread);
        m_pRefreshPrinterListThread = NULL;
    }
}

int EPUPrinterListDialog::DoModal()
{
    updatePrinterList();

    return exec();
}

void EPUPrinterListDialog::addPrinter(EPUPrinter* printer)
{
    if (!&printer)
    {
        return;
    }
    QStandardItem *item;
    item = new QStandardItem();
    item->setData( printer->GetPrinterName(), Qt::DisplayRole );
    if(printer->GetPrintCmd() == 1)
    {
        item->setData( QImage(":/Images/printer_status.png"), Qt::DecorationRole );
    }else
    {
        item->setData( QImage(":/Images/printer_status_laser.png"), Qt::DecorationRole );
    }
    item->setEditable( false );

    m_pSourceModel->appendRow(item);

}

void EPUPrinterListDialog::updatePrinterList()
{
    qDebug()<<"Get Printer List";
    m_pSourceModel->removeRows(0, m_pSourceModel->rowCount());
    EPUPrinterController* controller = EPUPrinterController::GetInstance();
    QVector<EPUPrinter*> prList = controller->GetPrinterList();
    for (int i = 0; i < prList.size(); i++)
    {
        EPUPrinter* pr = prList[i];
        addPrinter(pr);
    }
    EPUPrinter* ptr = controller->GetCurrentPrinter();
    int index = 0;
    if (ptr)
    {
        for(index = 0; index < prList.size(); index++)
        {
            if(ptr->GetModelName() == prList[index]->GetModelName() &&
                    ptr->GetPrinterId() == prList[index]->GetPrinterId())
            {
                break;
            }
        }
        qDebug()<<"current printer name: "<<ptr->GetPrinterName();     
    }
    qDebug()<<"index: "<<index;
    if (index >= 0 && index < prList.size())
    {
        QModelIndex modelIndex = m_pSourceModel->index(index, 0);
        qDebug()<<"update selection: "<<modelIndex;
        m_pSelectionModel->select(modelIndex, QItemSelectionModel::Select);
    }
}

EThreadReturnValue EPUPrinterListDialog::RunRefreshPrinterList(void* object, EThreadParam param)
{
    EPUPrinterListDialog* dialog = (EPUPrinterListDialog*)object;
    if (dialog)
    {
        dialog->runRefreshPrinterListLoop();
    }
    return 0;
}

void EPUPrinterListDialog::runRefreshPrinterListLoop()
{
    m_bKeepRefreshThreadUpdatePrListRunning = true;
    m_bRefreshThreadUpdatePrListStopped = false;

    EPUPrinterController* controller = EPUPrinterController::GetInstance();
    EPSPrinterController* controlLib = EPSPrinterController::GetInstance();
    Sleep(3000);
    /*controlLib->ReleaseInstance();
    controlLib->InitializeCommonLib();*/
    controller->FreeThreadUpdateStatus();
    controlLib->StartFindPrinter();
    controller->GetPrinterListFromLib();

    QApplication::postEvent(this, new QEvent((QEvent::Type)EPU_EVENT_TYPE_UPDATE_PRINTER_LIST));
}

void EPUPrinterListDialog::StartUpdatePrinterList()
{
    if (!m_bRefreshThreadUpdatePrListStopped)
    {
        return;
    }
    if (m_pRefreshPrinterListThread)
    {
        EThreadFree(m_pRefreshPrinterListThread);
        m_pRefreshPrinterListThread = NULL;
    }
    m_pRefreshPrinterListThread = EThreadCreateAndRun(EPUPrinterListDialog::RunRefreshPrinterList, (void*)this, EThreadParam());
}

bool EPUPrinterListDialog::event(QEvent *event)
{
    if (event)
    {
        switch (event->type())
        {
        case EPU_EVENT_TYPE_UPDATE_PRINTER_LIST:
        {
            qDebug()<<"update printer list";
            updatePrinterList();
            m_bKeepRefreshThreadUpdatePrListRunning = false;
            m_bRefreshThreadUpdatePrListStopped = true;
            break;
        }
        default:
            break;
        }
    }
    return QWidget::event(event);
}
void EPUPrinterListDialog::btnUpdateClicked()
{
    StartUpdatePrinterList();
}

void EPUPrinterListDialog::closeEvent(QCloseEvent *event)
{
    EPUPrinter* currentPrinter = EPUPrinterController::GetInstance()->GetCurrentPrinter();
    if (!currentPrinter)
    {
        btnOKClicked();
    }
    QDialog::closeEvent(event);
}

void EPUPrinterListDialog::btnOKClicked()
{
    QModelIndexList indexList = m_pListView->selectionModel()->selectedRows();
    int index = 0;
    if (indexList.size() > 0)
    {
        QModelIndex modelIndex = indexList[0];
        index = modelIndex.row();
        qDebug()<<"index ===== "<<index;
        EPUPrinterController::GetInstance()->UpdateCurrentPrinterWithIndex(index);
    }
    close();
}


