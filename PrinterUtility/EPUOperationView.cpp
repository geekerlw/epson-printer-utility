#include "EPUOperationView.h"

EPUOperationView::EPUOperationView(QWidget *parent)
    : EPUView(parent)
{
    m_pGuideLayout = new QVBoxLayout;
    m_pGuideLayout->setMargin(0);
    m_pGuideLayout->setSpacing(0);
    m_pGuideLayout->setAlignment(Qt::AlignTop);
    m_pMainLayout->addLayout(m_pGuideLayout);

    m_pGuideTitleLayout = new QHBoxLayout;
    m_pGuideTitleLayout->setMargin(0);
    m_pGuideTitleLayout->setSpacing(10);
    m_pGuideTitleLayout->addSpacing(10);
    m_pGuideTitleLayout->setAlignment(Qt::AlignLeft);

    m_pGuideLayout->addLayout(m_pGuideTitleLayout);

    m_pGuideIconView = new QLabel;
    m_pGuideIconView->setFixedSize(QSize(40, 40));
    m_pGuideTitleLayout->addWidget(m_pGuideIconView);

    m_pGuideLabel = new QLabel;
    m_pGuideLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pGuideLabel->setFixedHeight(40);
    m_pGuideTitleLayout->addWidget(m_pGuideLabel);
    QFont font;
    font.setFamily(font.defaultFamily());
    font.setBold(true);
    m_pGuideLabel->setFont(font);
}
