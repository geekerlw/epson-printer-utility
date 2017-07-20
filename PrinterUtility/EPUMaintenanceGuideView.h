#ifndef EPUMAINTENANCEGUIDEVIEW_H
#define EPUMAINTENANCEGUIDEVIEW_H

#include <QPushButton>
#include "EPUOperationView.h"
#include "EPUPrinterController.h"

enum
{
    EPU_MAINTENANCE_COMMAND_NONE = 0,
    EPU_MAINTENANCE_COMMAND_NOZZLE_CHECK,
    EPU_MAINTENANCE_COMMAND_HEAD_CLEANING
};

typedef int EPUMaintenanceCommand;

class EPUMaintenanceGuideView : public EPUOperationView
{
    Q_OBJECT
public:
    explicit EPUMaintenanceGuideView(QWidget *parent = 0);
    
public:
    EPUMaintenanceCommand GetMaintenanceCommand() {return m_nMaintenanceCommand;}
    void SetMaintenanceCommand(EPUMaintenanceCommand command);
    void SetPreView(EPUView* preView){m_pPreView = preView;}
signals:
    
public slots:
    void btnCancelClicked();
    void btnPrintClicked();
protected:
    EPUMaintenanceCommand   m_nMaintenanceCommand;

private:
    QLabel*                 m_pMaintenanceGuideLabel;
    QHBoxLayout*            m_pPrintButtonLayout;
    QPushButton*            m_pCancelButton;
    QPushButton*            m_pPrintButton;
    EPUView*                m_pPreView;

private:
    void setupConnection();
};

#endif // EPUMAINTENANCEGUIDEVIEW_H
