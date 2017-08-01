#ifndef EPUMAINVIEW_H
#define EPUMAINVIEW_H

#include "EPUView.h"
#include "EPUMainWindow.h"
#include <QTimer>
#include "EPUPrinterListDialog.h"
#include "EPUInkLevelView.h"

#define INKLOW 20

class EPUMainView : public EPUView
{
    Q_OBJECT
public:
    ~EPUMainView();
    explicit EPUMainView(QWidget *parent = 0);

public:
    void updateCurrenPrinterStaus();
    void updateCurrenInkJetPrinterStaus();
    void updateCurrenLaserPrinterStaus();
    QLabel* getStateLabel(){return m_pPrinterStateLabel;}
    QLabel* getStatusLabel(){return m_pPrinterStatusLabel;}
    QColor eprpColorToQColor(EPS_INT32 colorCode);
    QString eprpColorToQStringName(EPS_INT32 colorCode);
    void updateDefaultInklevel();
    void updatePrinterInkInfo();
    void updateCurrenPrinterDisconnected(bool first);
    void StatusLabelEmptyPrinterList();
	void PrinterLogoNormal();
    void PrinterLogoNormalLaser();
    void PrinterLogoWarning();
    void PrinterLogoWarningLaser();
    void PrinterLogoError();
    void PrinterLogoErrorLaser();
    void StopTimer(){m_timer->stop();}
    void StartTimer(){m_timer->start(2000);}
    void connection();
    void updatePrinterStatus();
    void updatePrinterState(int status, int error);

signals:
    
public slots:
    void btnShowPrinterListClicked();
    void setPrinterStatus(QString status){m_pPrinterStatusLabel->setText(status);}
    void setPrinterState(QString state){m_pPrinterStateLabel->setText(state);}
    void update();

private:
    void setupConnection();
private:
    QGroupBox*      m_pPrinterStatusBox;
    QHBoxLayout*    m_pPrinterStatusLayout;
    QVBoxLayout*    m_pUpdateStatusLayout;
    QLabel*         m_pPrinterLogo;
    QLabel*         m_pPrinterStateLabel;
    QLabel*         m_pPrinterStatusLabel;
    QLabel*         m_pPaperCountLabel;
    QPushButton*    m_pUpdateButton;
    QGroupBox*      m_pInkLevelsGroup;
    QVBoxLayout*    m_pInkViewLayout;
    QHBoxLayout*    m_pInkBox;
    QGroupBox*      m_pMaintenanceBox;
    QHBoxLayout*    m_pMaintenanceLayout;
    QTimer*         m_timer;
    QPushButton*    m_pPrinterListButton;
    QVector<EPUInkLevelView*> m_inkLevelViewArray;

    EPUPrinterListDialog* m_pPrinterListDlg;
};

#endif // EPUMAINVIEW_H
