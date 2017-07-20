#ifndef EPUPRINTERLISTDIALOG_H
#define EPUPRINTERLISTDIALOG_H

#include <QObject>
#include <QApplication>
#include <unistd.h>
#include <QListView>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListView>
#include <QPushButton>
#include "../EPUCommon/EPUCommonDef.h"
#include "EPUPrinter.h"
#include "../EPUCommon/EThread.h"

class EPUPrinterListDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EPUPrinterListDialog(QWidget *parent = 0);
    virtual ~EPUPrinterListDialog();

public:
    int DoModal();
    void StartUpdatePrinterList();
    static EThreadReturnValue RunRefreshPrinterList(void* object, EThreadParam param);
    void runRefreshPrinterListLoop();

private:
    void addPrinter(EPUPrinter* printer);

public:
    void updatePrinterList();
    
signals:
    
protected slots:
    void btnUpdateClicked();
    void btnOKClicked();

protected:
    bool event(QEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    EThread*            m_pRefreshPrinterListThread;
    QVBoxLayout*        m_pMainLayout;
    QListView*          m_pListView;
    QStandardItemModel* m_pSourceModel;
    QItemSelectionModel* m_pSelectionModel;

    QHBoxLayout*        m_pButtonLayout;
    QPushButton*        m_pUpdateButton;
    QPushButton*        m_pOKButton;

    bool                m_bKeepRefreshThreadUpdatePrListRunning;
    bool                m_bRefreshThreadUpdatePrListStopped;
};

#endif // EPUPRINTERLISTDIALOG_H
