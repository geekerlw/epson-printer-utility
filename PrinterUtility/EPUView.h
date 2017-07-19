#ifndef EPUVIEW_H
#define EPUVIEW_H

#include <QWidget>
#include "../EPUCommon/EPUCommonDef.h"
#include "EPUCallbackObject.h"

class EPUView : public QWidget
{
    Q_OBJECT
public:
    explicit EPUView(QWidget *parent = 0);
    
public:
    void SetDelegate(EPUCallbackObject* delegate) {m_pDelegate = delegate;}
    EPUCallbackObject* GetDelegate(){return m_pDelegate;}

    virtual void UpdateCurrentPrinter();
signals:
    
public slots:

protected:
    EPUCallbackObject*  m_pDelegate;
    QVBoxLayout*        m_pMainLayout;
    QHBoxLayout*        m_pPrinterNameLayout;
    QLabel*             m_pEpsonLogo;
    QLabel*             m_pPrinterNameLabel;
};

#endif // EPUVIEW_H
