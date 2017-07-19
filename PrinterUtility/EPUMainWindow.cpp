#include "EPUMainWindow.h"
#include <QFont>
#include "EPUGlobal.h"

EPUMainWindow::EPUMainWindow(QWidget* parent)
    : QWidget(parent)
    , m_pCurrentView(NULL)
{
    //EPUNotificationCenter* center = GetNotificationCenter();
    //center->AddObserver(this, "test notification", EPUMainWindow::InvokeFromNotification);
    m_pMaintenanceGuideView = new EPUMaintenanceGuideView;
    setupGUI();
    setupConnection();

}

EPUMainWindow::~EPUMainWindow()
{
}

void EPUMainWindow::setupGUI()
{
    m_pMainLayout = new QVBoxLayout(this);
    m_pMainLayout->setMargin(0);
    m_pMainLayout->setSpacing(0);
    m_pMainLayout->setAlignment(Qt::AlignTop);

    m_pToolbarLayout = new QHBoxLayout;
    m_pMainLayout->addLayout(m_pToolbarLayout);
    m_pToolbarLayout->setAlignment(Qt::AlignLeft);
    m_pToolbarLayout->setMargin(5);
    m_pToolbarLayout->setSpacing(5);

    m_pBuyEponInkButton = new QPushButton(QLabel::tr("Nozzle Check"));
    m_pBuyEponInkButton->setFixedSize(150, 80);
    m_pBuyEponInkButton->setFlat(true);
    m_pBuyEponInkButton->setObjectName("BuyEponInkButton");
    m_pBuyEponInkButton->setStyleSheet(
                "#BuyEponInkButton {"
                "border: none;"
                "outline: none;"
                "border-radius: 3px;"
                "background-color: rgba(255, 255, 255, 0);"
                "color: #FFFFFF;"
                "background-image: url(:/Images/Nozzle_small.png);"
                "background-position: top;"
                "background-repeat: none;"
                "text-align: bottom;"
                "font-size: 9pt;"
                "padding-right: 0px;"
                "padding-left: 0px;"
                "padding-bottom: 5px;"
                "}"

                "#BuyEponInkButton:hover {"
                "  background-color: rgba(255, 255, 255, 77);"
                "  color: #FFFFFF;"
                "  background-image: url(:/Images/Nozzle_small.png);"
                "}"
                "#BuyEponInkButton:pressed {"
                "  background-color: rgba(0, 0, 0, 77);"
                "  color: #B3B3B3;"
                "  background-image: url(:/Images/Nozzle_small.png);"
                "}"
                "#BuyEponInkButton:disabled {"
                "  color: #AD6E83;"
                "  background-image: url(:/Images/Nozzle_small.png);"
                "}"
                );
    /*QFont font = m_pBuyEponInkButton->font();
    font.setPointSize(50);
    m_pBuyEponInkButton->setFont(font);*/

    m_pToolbarLayout->addWidget(m_pBuyEponInkButton);


    m_pDriverSettingButton = new QPushButton(QLabel::tr("Head Cleaning"));
    m_pDriverSettingButton->setFixedSize(200, 80);
    m_pDriverSettingButton->setObjectName("DriverSettingButton");
    m_pDriverSettingButton->setStyleSheet(
                "#DriverSettingButton {"
                "border: none;"
                "outline: none;"
                "border-radius: 3px;"
                "background-color: rgba(255, 255, 255, 0);"
                "color: #FFFFFF;"
                "background-image: url(:/Images/Cleaning_small.png);"
                "background-position: top;"
                "background-repeat: none;"
                "text-align: bottom;"
                "font-size: 9pt;"
                "padding-right: 0px;"
                "padding-left: 0px;"
                "padding-bottom: 5px;"
                "}"

                "#DriverSettingButton:hover {"
                "  background-color: rgba(255, 255, 255, 77);"
                "  color: #FFFFFF;"
                "  background-image: url(:/Images/Cleaning_small.png);"
                "}"
                "#DriverSettingButton:pressed {"
                "  background-color: rgba(0, 0, 0, 77);"
                "  color: #B3B3B3;"
                "  background-image: url(:/Images/Cleaning_small.png);"
                "}"
                "#DriverSettingButton:disabled {"
                "  color: #AD6E83;"
                "  background-image: url(:/Images/Cleaning_small.png);"
                "}"
                );

    m_pToolbarLayout->addWidget(m_pDriverSettingButton);


#if 0
    m_pTestButton = new QPushButton(QLabel::tr("TEST"));
    m_pTestButton->setFixedSize(150, 80);
    m_pTestButton->setObjectName("DriverSettingButton");
    m_pTestButton->setStyleSheet(
                "#DriverSettingButton {"
                "border: none;"
                "outline: none;"
                "border-radius: 3px;"
                "background-color: rgba(255, 255, 255, 0);"
                "color: #FFFFFF;"
                "background-image: url(:/Images/Cleaning_small.png);"
                "background-position: top;"
                "background-repeat: none;"
                "text-align: bottom;"
                "font-size: 9pt;"
                "padding-right: 0px;"
                "padding-left: 0px;"
                "padding-bottom: 5px;"
                "}"

                "#DriverSettingButton:hover {"
                "  background-color: rgba(255, 255, 255, 77);"
                "  color: #FFFFFF;"
                "  background-image: url(:/Images/Cleaning_small.png);"
                "}"
                "#DriverSettingButton:pressed {"
                "  background-color: rgba(0, 0, 0, 77);"
                "  color: #B3B3B3;"
                "  background-image: url(:/Images/Cleaning_small.png);"
                "}"
                "#DriverSettingButton:disabled {"
                "  color: #AD6E83;"
                "  background-image: url(:/Images/Cleaning_small.png);"
                "}"
                );

    m_pToolbarLayout->addWidget(m_pTestButton);

#endif


    QWidget* bodyWidget = new QWidget;
    bodyWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    bodyWidget->setObjectName("Body");
    bodyWidget->setStyleSheet(
                "#Body {"
                "background-color: rgba(234, 234, 234, 255);"
                "}");
    m_pMainLayout->addWidget(bodyWidget);
    m_pBodyLayout = new QVBoxLayout;
    m_pBodyLayout->setMargin(0);
    m_pBodyLayout->setSpacing(0);
    m_pBodyLayout->setAlignment(Qt::AlignTop);
    bodyWidget->setLayout(m_pBodyLayout);

    setObjectName("MainWindow");
    setStyleSheet(
                "#MainWindow {"
                "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                "stop: 0.0 #971237,"
                "stop: 0.5 #730E2F,"
                "stop: 1.0 #520F25);"
                "}");
    setWindowTitle(tr("Epson Printer Utility"));
    setFixedSize(QSize(615, 500));

    QDesktopWidget *desktop = QApplication::desktop();

    int screenWidth, width;
    int screenHeight, height;
    int x, y;
    QSize windowSize;

    screenWidth = desktop->width(); // get width of screen
    screenHeight = desktop->height(); // get height of screen

    windowSize = size(); // size of our application window
    width = windowSize.width();
    height = windowSize.height();

    // little computations
    x = (screenWidth - width) / 2;
    y = (screenHeight - height) / 2;
    y -= 50;

    // move window to desired coordinates
    move ( x, y );

    setWindowIcon(QIcon(":/Images/AppIcon.png"));

    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
}

void EPUMainWindow::setupConnection()
{
    QObject::connect(m_pBuyEponInkButton, SIGNAL(clicked()), this, SLOT(btnBuyInkClicked()));
    QObject::connect(m_pDriverSettingButton, SIGNAL(clicked()), this, SLOT(btnDriverSettingClicked()));
}


void EPUMainWindow::btnBuyInkClicked()
{
    GetMainController()->StopMainViewTimer();
    m_pMaintenanceGuideView->SetMaintenanceCommand(EPU_MAINTENANCE_COMMAND_NOZZLE_CHECK);
    m_pMaintenanceGuideView->SetDelegate(m_pCurrentView->GetDelegate());
    ChangeCurrentViewToView(m_pMaintenanceGuideView);

    qDebug()<<__FUNCTION__;
}

void EPUMainWindow::btnDriverSettingClicked()
{
    GetMainController()->StopMainViewTimer();
    m_pMaintenanceGuideView->SetMaintenanceCommand(EPU_MAINTENANCE_COMMAND_HEAD_CLEANING);
    m_pMaintenanceGuideView->SetDelegate(m_pCurrentView->GetDelegate());
    ChangeCurrentViewToView(m_pMaintenanceGuideView);

    qDebug()<<__FUNCTION__;
}

void EPUMainWindow::ButtonDisable()
{
    m_pBuyEponInkButton->setEnabled(false);
    m_pDriverSettingButton->setEnabled(false);
}

void EPUMainWindow::HeadCleanButtonDisable()
{
    m_pDriverSettingButton->setEnabled(false);
}

void EPUMainWindow::NozzleCheckButtonAble()
{
    m_pBuyEponInkButton->setEnabled(true);
}

void EPUMainWindow::ButtonEnable()
{
    m_pBuyEponInkButton->setEnabled(true);
    m_pDriverSettingButton->setEnabled(true);
}

void EPUMainWindow::ChangeCurrentViewToView(EPUView *epuView)
{
    if (m_pCurrentView)
    {
        qDebug()<<"Remove current view: "<<m_pCurrentView;
        m_pBodyLayout->removeWidget(m_pCurrentView);
        m_pCurrentView->setHidden(true);
    }
    m_pCurrentView = epuView;
    qDebug()<<"Add new view: "<<m_pCurrentView;
    m_pBodyLayout->addWidget(m_pCurrentView);
    m_pCurrentView->setHidden(false);
    m_pCurrentView->UpdateCurrentPrinter();
}

void EPUMainWindow::UpdateCurrentPrinterForCurrentView()
{
    if (m_pCurrentView)
    {
        qDebug()<<"Update CurrentPrinter View";
        m_pCurrentView->UpdateCurrentPrinter();
    }
}

bool EPUMainWindow::event(QEvent *event)
{
    if (event)
    {
        switch (event->type())
        {
        case EPU_EVENT_TYPE_UPDATE_STATUS:
        {
            GetMainController()->UpdateCurrentPrinterStatus();
            break;
        }
        default:
            break;
        }
    }
    return QWidget::event(event);
}




