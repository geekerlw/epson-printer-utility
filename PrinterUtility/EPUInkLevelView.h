#ifndef EPUINKLEVELVIEW_H
#define EPUINKLEVELVIEW_H

#include <QWidget>
#include <QPainter>

class EPUInkLevelView : public QWidget
{
    //Q_OBJECT
public:
    explicit EPUInkLevelView(QColor color, const QString& inkName = QString::null, QWidget *parent = 0);

public:
    void SetInkColor(QColor color);
    void SetInkLevel(int inkLevel);
    void SetSmartCharge(bool smartcharge);
    void SetInkEndCartridge(int num, QString* name);
    void SetInkName(const QString& inkName);
    void SetInkInfo(QColor color, const QString& inkName, int inkLevel);
    
signals:
    
public slots:

protected:
    void paintEvent(QPaintEvent *event);

private:
    QColor              m_inkColor;
    QString             m_strInkName;
    int                 m_nInkLevel;
    bool                m_nSmartCharge;
    QString             m_strInkEndList[20];
	int					m_InkEndNum;
};

#endif // EPUINKLEVELVIEW_H
