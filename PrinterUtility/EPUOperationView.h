#ifndef EPUOPERATIONVIEW_H
#define EPUOPERATIONVIEW_H

#include <QObject>
#include "EPUView.h"

class EPUOperationView : public EPUView
{
    Q_OBJECT
public:
    explicit EPUOperationView(QWidget *parent = 0);
    
signals:
    
public slots:

protected:
    QVBoxLayout*            m_pGuideLayout;
    QHBoxLayout*            m_pGuideTitleLayout;
    QLabel*                 m_pGuideIconView;
    QLabel*                 m_pGuideLabel;
};

#endif // EPUOPERATIONVIEW_H
