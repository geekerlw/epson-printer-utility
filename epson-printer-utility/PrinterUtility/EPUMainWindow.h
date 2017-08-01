#ifndef EPUMAINWINDOW_H
#define EPUMAINWINDOW_H

#include <QDesktopWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include "../EPUCommon/EPUCommonDef.h"
#include "EPUView.h"
#include "EPUMaintenanceGuideView.h"

class EPUMainWindow: public QWidget
{
    Q_OBJECT
public:
    EPUMainWindow(QWidget* parent = 0);
    void ButtonDisable();
    void HeadCleanButtonDisable();
    void NozzleCheckButtonAble();
    void ButtonEnable();
    virtual ~EPUMainWindow();

public:
    void ChangeCurrentViewToView(EPUView* epuView);
    void UpdateCurrentPrinterForCurrentView();
    EPUMaintenanceGuideView* getMaintenanceGuideView(){return m_pMaintenanceGuideView;}

private:
    QVBoxLayout*        m_pMainLayout;
    QHBoxLayout*        m_pToolbarLayout;
    QPushButton*        m_pBuyEponInkButton;
    QPushButton*        m_pDriverSettingButton;
    QPushButton*        m_pTestButton;
    QVBoxLayout*        m_pBodyLayout;
    EPUView*            m_pCurrentView;
    EPUMaintenanceGuideView*    m_pMaintenanceGuideView;

private:
    void setupGUI();
    void setupConnection();

protected:
    bool event(QEvent *event);

protected slots:
    void btnBuyInkClicked();
    void btnDriverSettingClicked();
};

#endif // EPUMAINWINDOW_H
