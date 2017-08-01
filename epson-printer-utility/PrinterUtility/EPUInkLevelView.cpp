#include "EPUInkLevelView.h"
#include <QDebug>
#include "EPUPrinterController.h"
#include "EPUPrinter.h"
#include "EPUMainView.h"
#include "../EPSCommonLib/escpr-lib/inc/epson-escpr-err.h"

EPUInkLevelView::EPUInkLevelView(QColor color, const QString& inkName, QWidget *parent)
    : QWidget(parent)
    , m_strInkName(inkName)
    , m_inkColor(color)
    , m_nInkLevel(0)
    , m_nSmartCharge(FALSE)
	, m_InkEndNum(0)
{
}

void EPUInkLevelView::SetInkColor(QColor color)
{
    m_inkColor = color;
    update();
}

void EPUInkLevelView::SetInkLevel(int inkLevel)
{
    m_nInkLevel = inkLevel;
    update();
}

void EPUInkLevelView::SetSmartCharge(bool smartcharge){
	m_nSmartCharge = smartcharge;
    update();
}

void EPUInkLevelView::SetInkEndCartridge(int num, QString* name){
	m_InkEndNum = num;
	for(int i=0; i<num; i++){
		m_strInkEndList[i] = name[i];

		qDebug()<<"EndInkName: "<<m_strInkEndList[i];
	}
    update();
}

void EPUInkLevelView::SetInkName(const QString &inkName)
{
    m_strInkName = inkName;
    update();
}

void EPUInkLevelView::SetInkInfo(QColor color, const QString &inkName, int inkLevel)
{
    m_inkColor = color;
    m_strInkName = inkName;
    m_nInkLevel = inkLevel;
    update();
}

void EPUInkLevelView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

	int Draw_Inklevel=m_nInkLevel;


	QFont font;
	font.setFamily(font.defaultFamily());
	font.setPointSize(10);
	QFontMetrics metrics(font);
	painter.setFont(font);

	if(m_nSmartCharge == TRUE){ //
		if(m_InkEndNum ==0){
			painter.drawText(50, 30, QLabel::tr("Ink level information not available"));
		}else{
			for(int i=0; i < m_InkEndNum; i++){
				painter.drawText(50, 10+i*20, QLabel::tr("%1").arg(m_strInkEndList[i]));
			}
		}
	}else{
		QSize frameSize = size();
		int selfWidth = frameSize.width();
		int selfHeight = frameSize.height();


	//  
		if(m_nInkLevel <= 3) Draw_Inklevel = 3;

		int w = 30;
		int h = 100; //100 is also 100%
		int x = selfWidth / 2 - w / 2;
		int y = selfHeight / 2 - h / 2 - 20;

		int leftSize = 13;

		painter.setPen(Qt::NoPen);
		QColor middleColor = QColor::fromRgb(250, 250, 250);

		QLinearGradient outFristGrad(QPointF(x, y), QPointF(x + leftSize, y));
		outFristGrad.setColorAt(0.0, Qt::gray);
		outFristGrad.setColorAt(1.0, middleColor);

		painter.setBrush(outFristGrad);

		painter.drawRect(x, y, leftSize, h - Draw_Inklevel);

		QLinearGradient outSecondGrad(QPointF(x + leftSize, y), QPointF(x + w, y));
		outSecondGrad.setColorAt(0.0, middleColor);
		outSecondGrad.setColorAt(1.0, Qt::gray);

		painter.setBrush(outSecondGrad);

		painter.drawRect(x + leftSize, y, w - leftSize, h - Draw_Inklevel);

		QLinearGradient firstGrad(QPointF(x, y), QPointF(x + leftSize, y));
		firstGrad.setColorAt(0.0, m_inkColor);
		firstGrad.setColorAt(1.0, middleColor);

		painter.setBrush(firstGrad);

		painter.drawRect(x, y + (h - Draw_Inklevel), leftSize, Draw_Inklevel);

		QLinearGradient secondGrad(QPointF(x + leftSize, y), QPointF(x + w, y));
		secondGrad.setColorAt(0.0, middleColor);
		secondGrad.setColorAt(1.0, m_inkColor);

		painter.setBrush(secondGrad);

		painter.drawRect(x + leftSize, y + (h - Draw_Inklevel), w - leftSize, Draw_Inklevel);

		painter.setBrush(Qt::NoBrush);
		QPen pen(QColor::fromRgb(50, 50, 50));
		pen.setWidth(1);

		painter.setPen(pen);

		painter.drawLine(QPointF(x, y + (h - Draw_Inklevel)), QPointF(x + w, y + (h - Draw_Inklevel)));
		painter.drawRect(x, y, w, h);

		QFont font;
		font.setFamily(font.defaultFamily());
		font.setPointSize(10);
		QFontMetrics metrics(font);
		int textWidth = metrics.width(m_strInkName);
		int textHeight = metrics.height();

		painter.setFont(font);
		painter.drawText(selfWidth / 2 - textWidth / 2, y + h + textHeight, m_strInkName);

	 	 EPUPrinterController* controller = EPUPrinterController::GetInstance();
		 EPUPrinter* printer = controller->GetCurrentPrinter();
		 if(printer)
		{

		   if(!printer->GetPrinterDisconnected()
		           && !(printer->GetPrinterStatus().errorCode == EPS_PRNERR_FACTORY)
		           && !(printer->GetPrinterStatus().errorCode == EPS_PRNERR_CEMPTY)
		           && !(printer->GetPrinterStatus().errorCode == EPS_PRNERR_CFAIL))
		    {

		        QString inkLevelText = QString::number(Draw_Inklevel);
				textWidth = metrics.width(inkLevelText);
	//            painter.drawText(selfWidth / 2 - textWidth / 2, y + h + 2 * textHeight, inkLevelText);

		    }

		}

	

		if(m_nInkLevel == 0 && !m_strInkName.isEmpty())
		{
		    QPixmap pix_out(":/Images/Inkout.png");
		    painter.setBrush(QBrush(pix_out));
		    painter.drawPixmap((selfWidth - 32)/2, h/2 + 17, pix_out);
		}

		if(m_nInkLevel >= 1 && m_nInkLevel <= INKLOW)
		{
		    //qDebug()<<"show inklow image";
		    QPixmap pix_low(":/Images/Inklow.png");
		    painter.setBrush(QBrush(pix_low));
		    painter.drawPixmap((selfWidth - 32)/2, h/2 + 10, pix_low);
		}
		
	}


}


