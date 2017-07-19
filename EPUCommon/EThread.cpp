#include "EThread.h"
#include <QDebug>

#define START_DELAY_TIME (1000)
#define MAX_WAIT_CLOSE_THREAD (10*1000)

QList<EThread*> EThread::m_threadList = QList<EThread*>();
bool EThread::m_bStopAllThread        = false;
QMutex EThread::m_mutex;

EThread::EThread(QObject *parent)
    : QThread(parent)
    , m_threadProc()
    , m_threadParam()
    , m_threadObject(NULL)
    , m_bIsReadyForHandling(false)
    , m_threadPriority()
    , m_nRetValue(0)
{
}

void EThread::run()
{
    if (m_threadProc && m_threadObject)
    {
        m_nRetValue = m_threadProc(m_threadObject, m_threadParam);
    }
}

EThreadReturnValue EThread::getThreadReturnValue()
{
    return m_nRetValue;
}

void EThread::msleep(unsigned long timeOut)
{
    QThread::msleep(timeOut);
}

bool EThread::stopAllThread()
{
    bool ret(true);
    if (true)
    {
        QMutexLocker lock(&m_mutex);
        m_bStopAllThread = true;
    }
    QThread::msleep(100);

    //wait all thread stopped function.
    for (int i = 0; i < m_threadList.size(); i++)
    {
        EThread* thread = m_threadList.at(i);
        if(thread)
        {
            if(!thread->wait(MAX_WAIT_CLOSE_THREAD))
                ret = false;
        }
    }
    return ret;
}

bool EThread::isAppExitting()
{
    QMutexLocker lock(&m_mutex);
    return m_bStopAllThread;
}

bool EThread::isReadyForHandling()
{
    return m_bIsReadyForHandling;
}

void EThread::setReadyForHandling(bool flag)
{
    m_bIsReadyForHandling = flag;
}

EThread *EThreadCreateAndRun(EThreadProc proc, void *object, EThreadParam param, bool isRunImmediately)
{
    EThread *thread = new EThread();
    thread->m_threadProc = proc;
    thread->m_threadObject = object;
    thread->m_threadParam = param;
    thread->setTerminationEnabled(true);
    {
        QMutexLocker lock(&EThread::m_mutex);
        EThread::m_threadList.push_back(thread);
    }
    if (isRunImmediately)
    {
        thread->m_threadPriority = (QThread::Priority)(QThread::InheritPriority - EThread::m_threadList.size() % QThread::InheritPriority);
        thread->start(thread->m_threadPriority);
    }
    return thread;
}

void EThreadRun(EThread *thread)
{
    thread->start();
}

int EThreadFree(EThread *thread)
{
    qDebug()<<"Step wait 00001";
    if (thread)
    {
        qDebug()<<"Step wait 00002";
        thread->wait(MAX_WAIT_CLOSE_THREAD);
        if (EThread::m_threadList.contains(thread))
        {
            qDebug()<<"Lock Thread !!!!!!!!!!!";
            QMutexLocker lock(&EThread::m_mutex);
            //EThread::m_threadList.removeOne(thread);
        }


        qDebug()<<"check call function delete thread!!!!!!!!!!!!!!!!!!!!!!!!!";

        delete thread;
        thread = NULL;
    }
    return 0;
}
